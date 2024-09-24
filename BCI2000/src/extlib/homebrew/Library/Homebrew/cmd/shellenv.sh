# Documentation defined in Library/Homebrew/cmd/shellenv.rb

# HOMEBREW_CELLAR and HOMEBREW_PREFIX are set by extend/ENV/super.rb
# HOMEBREW_REPOSITORY is set by bin/brew
# Leading colon in MANPATH prepends default man dirs to search path in Linux and macOS.
# Please do not submit PRs to remove it!
# shellcheck disable=SC2154
homebrew-shellenv() {
  if [[ "${HOMEBREW_PATH%%:"${HOMEBREW_PREFIX}"/sbin*}" == "${HOMEBREW_PREFIX}/bin" ]]
  then
    return
  fi

  if [[ -n "$1" ]]
  then
    HOMEBREW_SHELL_NAME="$1"
  else
    HOMEBREW_SHELL_NAME="$(/bin/ps -p "${PPID}" -c -o comm=)"
  fi

  if [[ -n "${HOMEBREW_MACOS}" ]] &&
     [[ "${HOMEBREW_MACOS_VERSION_NUMERIC}" -ge "140000" ]] &&
     [[ -x /usr/libexec/path_helper ]]
  then
    HOMEBREW_PATHS_FILE="${HOMEBREW_PREFIX}/etc/paths"

    if [[ ! -f "${HOMEBREW_PATHS_FILE}" ]]
    then
      printf '%s/bin\n%s/sbin\n' "${HOMEBREW_PREFIX}" "${HOMEBREW_PREFIX}" 2>/dev/null >"${HOMEBREW_PATHS_FILE}"
    fi

    if [[ -r "${HOMEBREW_PATHS_FILE}" ]]
    then
      PATH_HELPER_ROOT="${HOMEBREW_PREFIX}"
    fi
  fi

  case "${HOMEBREW_SHELL_NAME}" in
    fish | -fish)
      echo "set --global --export HOMEBREW_PREFIX \"${HOMEBREW_PREFIX}\";"
      echo "set --global --export HOMEBREW_CELLAR \"${HOMEBREW_CELLAR}\";"
      echo "set --global --export HOMEBREW_REPOSITORY \"${HOMEBREW_REPOSITORY}\";"
      echo "fish_add_path --global --move --path \"${HOMEBREW_PREFIX}/bin\" \"${HOMEBREW_PREFIX}/sbin\";"
      echo "if test -n \"\$MANPATH[1]\"; set --global --export MANPATH '' \$MANPATH; end;"
      echo "if not contains \"${HOMEBREW_PREFIX}/share/info\" \$INFOPATH; set --global --export INFOPATH \"${HOMEBREW_PREFIX}/share/info\" \$INFOPATH; end;"
      if [[ -n "${HOMEBREW_LINUX}" ]]
      then
        echo "if test -n \"\$XDG_DATA_DIRS\"; set --global --export XDG_DATA_DIRS \"${HOMEBREW_PREFIX}/share\" \$XDG_DATA_DIRS; end;"
      fi
      ;;
    csh | -csh | tcsh | -tcsh)
      echo "setenv HOMEBREW_PREFIX ${HOMEBREW_PREFIX};"
      echo "setenv HOMEBREW_CELLAR ${HOMEBREW_CELLAR};"
      echo "setenv HOMEBREW_REPOSITORY ${HOMEBREW_REPOSITORY};"
      if [[ -n "${PATH_HELPER_ROOT}" ]]
      then
        PATH_HELPER_ROOT="${PATH_HELPER_ROOT}" PATH="${HOMEBREW_PATH}" /usr/libexec/path_helper -c
      else
        echo "setenv PATH ${HOMEBREW_PREFIX}/bin:${HOMEBREW_PREFIX}/sbin:\$PATH;"
      fi
      echo "test \${?MANPATH} -eq 1 && setenv MANPATH :\${MANPATH};"
      echo "setenv INFOPATH ${HOMEBREW_PREFIX}/share/info\`test \${?INFOPATH} -eq 1 && echo :\${INFOPATH}\`;"
      if [[ -n "${HOMEBREW_LINUX}" ]]
      then
        echo "test \${?XDG_DATA_DIRS} -eq 1 && setenv XDG_DATA_DIRS \"${HOMEBREW_PREFIX}/share:\$XDG_DATA_DIRS\";"
      fi
      ;;
    pwsh | -pwsh | pwsh-preview | -pwsh-preview)
      echo "[System.Environment]::SetEnvironmentVariable('HOMEBREW_PREFIX','${HOMEBREW_PREFIX}',[System.EnvironmentVariableTarget]::Process)"
      echo "[System.Environment]::SetEnvironmentVariable('HOMEBREW_CELLAR','${HOMEBREW_CELLAR}',[System.EnvironmentVariableTarget]::Process)"
      echo "[System.Environment]::SetEnvironmentVariable('HOMEBREW_REPOSITORY','${HOMEBREW_REPOSITORY}',[System.EnvironmentVariableTarget]::Process)"
      echo "[System.Environment]::SetEnvironmentVariable('PATH',\$('${HOMEBREW_PREFIX}/bin:${HOMEBREW_PREFIX}/sbin:'+\$ENV:PATH),[System.EnvironmentVariableTarget]::Process)"
      echo "[System.Environment]::SetEnvironmentVariable('MANPATH',\$('${HOMEBREW_PREFIX}/share/man'+\$(if(\${ENV:MANPATH}){':'+\${ENV:MANPATH}})+':'),[System.EnvironmentVariableTarget]::Process)"
      echo "[System.Environment]::SetEnvironmentVariable('INFOPATH',\$('${HOMEBREW_PREFIX}/share/info'+\$(if(\${ENV:INFOPATH}){':'+\${ENV:INFOPATH}})),[System.EnvironmentVariableTarget]::Process)"
      if [[ -n "${HOMEBREW_LINUX}" ]]
      then
        echo "if(\${ENV:XDG_DATA_DIRS}){[System.Environment]::SetEnvironmentVariable('XDG_DATA_DIRS',\$('${HOMEBREW_PREFIX}/share:'+\${ENV:XDG_DATA_DIRS}),[System.EnvironmentVariableTarget]::Process)}"
      fi
      ;;
    *)
      echo "export HOMEBREW_PREFIX=\"${HOMEBREW_PREFIX}\";"
      echo "export HOMEBREW_CELLAR=\"${HOMEBREW_CELLAR}\";"
      echo "export HOMEBREW_REPOSITORY=\"${HOMEBREW_REPOSITORY}\";"
      if [[ -n "${PATH_HELPER_ROOT}" ]]
      then
        PATH_HELPER_ROOT="${PATH_HELPER_ROOT}" PATH="${HOMEBREW_PATH}" /usr/libexec/path_helper -s
      else
        echo "export PATH=\"${HOMEBREW_PREFIX}/bin:${HOMEBREW_PREFIX}/sbin\${PATH+:\$PATH}\";"
      fi
      echo "[ -z \"\${MANPATH-}\" ] || export MANPATH=\":\${MANPATH#:}\";"
      echo "export INFOPATH=\"${HOMEBREW_PREFIX}/share/info:\${INFOPATH:-}\";"
      if [[ -n "${HOMEBREW_LINUX}" ]]
      then
        echo "[ -z \"\${XDG_DATA_DIRS-}\" ] || export XDG_DATA_DIRS=\"${HOMEBREW_PREFIX}/share:\${XDG_DATA_DIRS-}\";"
      fi
      ;;
  esac
}
