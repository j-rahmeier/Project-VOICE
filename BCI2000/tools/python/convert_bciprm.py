import os

def convert_bciprm( params, outfilepath ):
    if not os.path.isdir( os.path.dirname( outfilepath ) ): 
        os.makedirs( os.path.dirname( outfilepath ) )
    outfile = open( outfilepath, "w" )

    for c in params:
        for sub in params[ c ]:
            for p in params[ c ][ sub ]:
                outfile.write( f"{ c }:{ sub } { params[ c ][ sub ][ p ][ 'Type' ] } { p }= { params[ c ][ sub ][ p ] } // { params[ c ][ sub ][ p ][ 'Comment' ] } { chr( 10 ) }" )

    outfile.close()

def read_bciprm( filepath ): 
    params = BCIParamList()
    f = open( filepath, "r" )
    l = f.readline()
    while l:
        section, l = l.split( ":", 1 )
        subsection, l = l.split( " ", 1 )
        t, l = l.split( " ", 1 )
        name, l = l.split( " ", 1 )
        name = name[ : -1 ]

        column_labels = ""
        row_labels = ""
        value = "" 
        comment = ""
        default = ""
        min_val = ""
        max_val = ""

        if t == "matrix":
            v = [ None, None, None ]
            l = l.strip()
            if "{" == l[ 0 ]:
                v[ 0 ], l = l.split( "}", 1 )
                v[ 0 ] = v[ 0 ].strip( "{" ).split()
            else:
               v[ 0 ] = l[ 0 ] 
               l = l[ 1: ]

            l = l.strip()
            if "{" == l[ 0 ]:
                v[ 1 ], l = l.split( "}", 1 )
                v[ 1 ] = v[ 1 ].strip( "{" ).split()
            else:
               v[ 1 ] = l[ 0 ] 
               l = l[ 1: ]

            s = l.split( "//" )[ 0 ].split()
            v[ 2 ]  = [ s[ i: i + len( v[ 0 ] ) ] for i in range( 0, len( s ), len( v[ 0 ] ) ) ]

            row_labels, column_labels, value = v

        elif "list" in t:
            value = l.split( "//" )[ 0 ].split()[ 1: ]

        elif t == "string":
            value = l.split( "//" )[ 0 ]

        else:
            v = l.split( "//" )[ 0 ].split()
            if len( v ) == 1:
                value = v[ 0 ]
            else:
                value, default, min_val, max_val = v

        if len( l.split( "//" ) ) > 1:
            comment = l.split( "//" )[ 1 ]
            if comment[ -1 ] == '\n': 
                comment = comment[ : -1 ]
                
        params[ section ][ subsection ][ name ][ "Section" ] = section
        params[ section ][ subsection ][ name ][ "Type" ] = t
        params[ section ][ subsection ][ name ][ "DefaultValue" ] = default
        params[ section ][ subsection ][ name ][ "LowRange" ] = min_val
        params[ section ][ subsection ][ name ][ "HighRange" ] = max_val
        params[ section ][ subsection ][ name ][ "Comment" ] = comment
        params[ section ][ subsection ][ name ][ "Value" ] = value
        params[ section ][ subsection ][ name ][ "RowLabels" ] = row_labels
        params[ section ][ subsection ][ name ][ "ColumnLabels" ] = column_labels
        
        l = f.readline()

    return params

class BCIParamList( dict ):
    def __getitem__( self, item ):
        item = item.replace( " ", "%20" ).strip()
        if not item in self.keys():
            print( f"Creating New Section: { item }" )
            super().__setitem__( item, BCIParamSection() )
        return  super().__getitem__( item )

    def __getattr__( self, item ):
        return self.__getitem__( item )

    def __setattr__( self, item, val ):
        self.__setitem__( item, val )

class BCIParamSection( dict ):
    def __getitem__( self, item ): 
        item = item.replace( " ", "%20" ).strip()
        if not item in self.keys():
            print( f"Creating New Sub Section: { item }" )
            super().__setitem__( item, BCIParamSubSection() )
        return super().__getitem__( item )

    def __getattr__( self, item ):
        return self.__getitem__( item )

    def __setattr__( self, item, val ):
        self.__setitem__( item, val )

class BCIParamSubSection( dict ):
    def __getitem__( self, item ): 
        item = item.replace( " ", "%20" ).strip()
        if not item in self.keys():
            print( f"Creating New Parameter: { item }" )
            super().__setitem__( item, BCIParam() )
        return super().__getitem__( item )

    def __getattr__( self, item ):
        return self.__getitem__( item )

    def __setattr__( self, item, val ):
        self.__setitem__( item, val )
        
class BCIParam( dict ):
    def __init__( self ):
        self[ "Section" ] = ""
        self[ "Type" ] = ""
        self[ "DefaultValue" ] = ""
        self[ "LowRange" ] = ""
        self[ "HighRange" ] = ""
        self[ "Comment" ] = ""
        self[ "Value" ] = ""
        self[ "RowLabels" ] = ""
        self[ "ColumnLabels" ] = ""

    def __getitem__( self, item ):
        item = item.strip().replace( " ", "%20" )
        return super().__getitem__( item )

    def __setitem__( self, item, val ):
        item = item.strip().replace( " ", "%20" )
        return  super().__setitem__( item, val )

    def __getattr__( self, item ):
        return self.__getitem__( item )

    def __setattr__( self, item, val ):
        self.__setitem__( item, val )

    def __str__( self ):
        if self[ "Type" ] == "matrix": 
            s = ""

            if type( self[ "RowLabels" ] ) is list: 
                s += "{ "
                for i in self[ "RowLabels" ]:
                    s += ( i if len( i ) > 0 else "%" ) + " "
                s += "} "
            else:
                s += self[ "RowLabels" ] + " "

            if type( self[ "ColumnLabels" ] ) is list: 
                s += "{ "
                for i in self[ "ColumnLabels" ]:
                    s += ( i if len( i ) > 0 else "%" ).replace( " ", "%20" ) + " "
                s += "} "
            else:
                s += self[ "ColumnLabels" ] + " "

            if len( self[ "Value" ] ) > 0: 
                for q in range( len( self[ "Value" ] ) ):
                    for i in range( len( self[ "Value" ][ 0 ] ) ):
                        l = self[ "Value" ][ q ][ i ]
                        s += ( l if len( l ) > 0 else "%" ).replace( " ", "%20" ) + " "
            return s

        if "list" in self[ "Type" ]:
            return " ".join( [ str( len( self[ "Value" ] ) ) ] + self[ "Value" ] )

        self[ "Value" ] = self[ "Value" ] if len( self[ "Value" ] ) > 0 else "%"
        self[ "DefaultValue" ] = self[ "DefaultValue" ] if len( self[ "Value" ] ) > 0 else "%"
        self[ "LowRange" ] = self[ "LowRange" ] if len( self[ "Value" ] ) > 0 else "%"
        self[ "HighRange" ] = self[ "HighRange" ] if len( self[ "Value" ] ) > 0 else "%"

        return " ".join( [ self[ "Value" ] if not type( self[ "Value" ] ) is list else self[ "Value" ][ 0 ], self[ "DefaultValue" ], self[ "LowRange" ], self[ "HighRange" ] ] ).strip();

class BCISettings( dict ): 
    def __getitem__( self, item ):
        item = item.replace( " ", "%20" ).strip()
        if not item in self.keys():
            print( f"Creating New Setting: { item }" )
            super().__setitem__( item, None )
        return super().__getitem__( item )

    def __setitem__( self, item, val ):
        item = item.strip().replace( " ", "%20" )
        super().__setitem__( item, val )

    def __getattr__( self, item ):
        return self.__getitem__( item )

    def __setattr__( self, item, val ):
        self.__setitem__( item, val )

def bcimatrix( n, m ):
    return [ [ "" for _ in range( m ) ] for __ in range( n ) ]

def bcilist( n ):
    return [ "" for _ in range( n ) ]
