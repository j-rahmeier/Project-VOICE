////////////////////////////////////////////////////////////////////////////////
// $Id: ComRegistrar.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description:
//   A class that interprets ATL COM type registry scripts. It is meant to
//   to process the contents of VisualStudio-generated .rgs files but may
//   fail to do so; use at your own risk.
//
//   In addition to the %MODULE% variable in ATL scripts, the following
//   variables are supported:
//   %MODULEPATH% -- the directory the module resides in
//   %MODULEPATH,1% -- the parent directory of the module directory
//   %MODULEPATH,2% -- the parent of the parent directory of the module directory
//
//    To specify a string value as a REG_EXPAND_SZ value rather than REG_SZ,
//    prepend the string with an "e" rather than an "s".
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "ComRegistrar.h"
#include <Olectl.h>

using namespace com;

static LONG
DeleteSubentries( HKEY inKey )
{
  LONG result = ERROR_SUCCESS;
  char nameBuffer[1024];
  DWORD length = sizeof( nameBuffer ) / sizeof( *nameBuffer );
  DWORD idx = 0;
  while( ERROR_SUCCESS == ::RegEnumValueA( inKey, idx++, nameBuffer, &length, NULL, NULL, NULL, NULL ) )
  {
    LONG subResult = ::RegDeleteValueA( inKey, nameBuffer );
    if( result == ERROR_SUCCESS )
      result = subResult;
  }
  idx = 0;
  length = sizeof( nameBuffer ) / sizeof( *nameBuffer );
  while( ERROR_SUCCESS == ::RegEnumKeyExA( inKey, idx++, nameBuffer, &length, NULL, NULL, NULL, NULL ) )
  {
    HKEY subkey = NULL;
    if ( ERROR_SUCCESS == ::RegOpenKeyA(inKey, nameBuffer, &subkey))
      DeleteSubentries(subkey);
    LONG subResult = ::RegDeleteKeyA( inKey, nameBuffer );
    if( result == ERROR_SUCCESS )
      result = subResult;
  }
  return result;
}

bool
Registrar::Parse( const std::string& inScript, const Environment& inEnvironment )
{
  delete mpRootNode;
  mpRootNode = new Node();
  std::istringstream iss( inScript );
  return Parse( iss, mpRootNode, inEnvironment );
}

bool
Registrar::Parse(std::istream& inStream, Node* inCurNode, const Environment& inEnvironment )
{
  std::string line;
  std::getline( inStream, line );
  std::istringstream iss( line );

  Token token( inEnvironment );
  bool isValue = false;
  int flags = Key::Default;
  std::string name, value;
  iss >> token;
  if( token == "val" )
  {
    isValue = true;
    iss >> token;
  }
  else if( token == "NoRemove" )
  {
    flags |= Key::NoRemove;
    iss >> token;
  }
  else if( token == "ForceRemove" )
  {
    flags |= Key::ForceRemove;
    iss >> token;
  }
  name = token;
  if( name.empty() )
    return false;

  iss >> token;
  if( token == "=" )
  {
    flags |= Key::SetValue;
    iss >> token;
    if( token == "e" )
      flags |= Key::Expand;
    else if( token != "s" )
      return false;
    iss >> token;
    value = token;
    if( value.empty() )
      return false;
  }

  Node* pNode = NULL;
  if( isValue )
    pNode = new Value( name, value );
  else
    pNode = new Key( name, value, flags );
  inCurNode->AddChild( pNode );

  if( ( inStream >> std::ws ).peek() == '{' )
  {
    inStream.get();
    while( ( inStream >> std::ws ).peek() != '}' )
      if( !Parse( inStream, pNode, inEnvironment ) )
        return false;
    inStream.get();
  }

  return !!inStream;
}

std::istream&
Registrar::Token::ExtractFrom( std::istream& is )
{
  this->clear();
  is >> std::ws;
  if( is.peek() == '\'' )
    std::getline( is.ignore(), *this, '\'' );
  else
    is >> static_cast<std::string&>( *this );

  // copy content into result, parsing out environment variables
  std::string result, name;
  bool inpercent = false;
  for (auto& c : *this)
  {
      switch (c)
      {
          case '%':
            inpercent = !inpercent;
            if (!inpercent)
            {
                // name finished, replace with variable
                if (name.empty())
                {
                    result += "%";
                }
                else
                {
                    auto i = mrEnvironment.find(name);
                    if (i != mrEnvironment.end())
                        result += i->second;
                }
                name.clear();
            }
            break;
        default:
            if (inpercent)
                name += c;
            else
                result += c;
      }
  }
  this->assign(result);
  return is;
}

HRESULT
Registrar::Node::Execute( HKEY inParent, int inAction ) const
{
  HRESULT result = S_OK;
  std::vector<Node*>::const_iterator i = mNodes.begin();
  while( i != mNodes.end() )
  {
    HRESULT subResult = ( *i++ )->Execute( inParent, inAction );
    if( result == S_OK )
      result = subResult;
  }
  return result;
}

void
Registrar::Node::Print( std::ostream& os ) const
{
  if( mNodes.empty() )
    return;

  os << "{\n";
  for( size_t i = 0; i < mNodes.size(); ++i )
    mNodes[i]->Print( os );
  os << "}\n";
}

HRESULT
Registrar::Key::Execute( HKEY inParent, int inAction ) const
{
  HKEY key = inParent;
  LONG error = ERROR_SUCCESS;
  if( key == 0 )
  {
    if( mName == "HKCR" )
      key = HKEY_CLASSES_ROOT;
    else if( mName == "HKCC" )
      key = HKEY_CURRENT_CONFIG;
    else if( mName == "HKCU" )
      key = HKEY_CURRENT_USER;
    else if( mName == "HKLM" )
      key = HKEY_LOCAL_MACHINE;
    else if( mName == "HKU" )
      key = HKEY_USERS;
    else
      return ERROR_BADKEY;
    error = ::RegOpenKeyExA( key, NULL, 0, KEY_ALL_ACCESS, &key );
  }
  else
  {
    error = ::RegCreateKeyExA( key, mName.c_str(), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, NULL );
    if (ERROR_SUCCESS == error && (mFlags & SetValue) && (inAction & Create))
    {
      DWORD type = (mFlags & Expand) ? REG_EXPAND_SZ : REG_SZ;
      error = ::RegSetValueExA(key, NULL, 0, type, reinterpret_cast<const BYTE*>(mValue.c_str()), static_cast<DWORD>(mValue.length() + 1));
    }
  }
  if( error != ERROR_SUCCESS )
    return SELFREG_E_CLASS;

  HRESULT result = Node::Execute( key, inAction );
  if( inParent )
  {
    error = ERROR_SUCCESS;
    if( ( inAction & Remove ) && ( mFlags & ForceRemove ) )
      error = DeleteSubentries( key );
    ::RegCloseKey( key );
    if( error == ERROR_SUCCESS && ( inAction & Remove ) && !( mFlags & NoRemove ) )
    {
      error = ::RegDeleteKeyA( inParent, mName.c_str() );
      if( error != ERROR_SUCCESS && result == S_OK )
        result = S_FALSE;
    }
    if( result == S_OK && error != ERROR_SUCCESS )
      result = SELFREG_E_CLASS;
  }
  return result;
}

void
Registrar::Key::Print( std::ostream& os ) const
{
  if( mFlags & NoRemove )
    os << "NoRemove ";
  if( mFlags & ForceRemove )
    os << "ForceRemove ";
  os << "'" << mName << "'";
  if (mFlags & SetValue)
  {
    std::string type = (mFlags & Expand) ? "e" : "s";
    os << " = " << type << " '" << mValue << "'";
  }
  os << '\n';
  Node::Print( os );
}

HRESULT
Registrar::Value::Execute( HKEY inParent, int inAction ) const
{
  LONG error = ERROR_SUCCESS;
  if( inAction & Create )
    error = ::RegSetValueExA( inParent, mName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>( mValue.c_str() ), static_cast<DWORD>( mValue.length() + 1 ) );
  if( inAction & Remove )
    ::RegDeleteValueA( inParent, mName.c_str() ); // no error on nonexisting value
  return error == ERROR_SUCCESS ? S_OK : SELFREG_E_CLASS;
}
