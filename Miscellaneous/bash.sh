# Function to check WSL distros and return the current distro name (if multiple exist)
# get_wsl_distro_prompt() {
#     # Check if we're running in WSL (skip if not)
#     if [ ! -f /proc/sys/fs/binfmt_misc/WSLInterop ]; then
#         return
#     fi

#     # Get list of installed WSL distros (skip errors if 'wsl.exe' is unavailable)
#     local wsl_distros
#     wsl_distros=$(wsl.exe --list --quiet 2>/dev/null | grep -v -e '^[[:space:]]*$')

#     # Count distros (if 'wsl.exe' works)
#     if [ -n "$wsl_distros" ]; then
#         local distro_count
#         distro_count=$(echo "$wsl_distros" | wc -l)

#         # Only proceed if multiple distros exist
#         if [ "$distro_count" -gt 1 ]; then
#             # Get current distro name (fallback methods)
#             local current_distro
#             if [ -n "$WSL_DISTRO_NAME" ]; then
#                 current_distro="$WSL_DISTRO_NAME"
#             else
#                 current_distro=$(grep -oP '^NAME="\K[^"]+' /etc/os-release | sed 's/ /_/g')
#             fi

#             # Format as [Distro_Name]
#             echo "[${current_distro}] "
#         fi
#     fi
# }

# Set PS1 (with yellow distro name if applicable)
# distro_name() {
#     wsl_distros=$(wsl.exe --list --quiet | iconv -f UTF-16LE -t UTF-8 | tr -d '\r')

#     if [ -n "$wsl_distros" ]; then
#         count=$(echo "$wsl_distros" | wc -l)
#         #echo $dis
#         current_distro="";
#         if [ -n "$WSL_DISTRO_NAME" ]; then
#             # echo $WSL_DISTRO_NAME
#             current_distro="$WSL_DISTRO_NAME"
#         fi

#         # echo $current_distro
#         return $current_distro
#         # echo ${wsl_distros}
#     fi
# }

# echo "[\[\033[1;31m\]\$(. /etc/os-release; echo \${distro_name})\[\033[0m\]]:\u@\h>\[\033[38:5:41m\]\W\[\033[0m\]\$"

distro_name() {
    wsl_distros=$(wsl.exe --list --quiet | iconv -f UTF-16LE -t UTF-8 | tr -d '\r')

    if [ -n "$wsl_distros" ]; then
        current_distro=""
        distro_number=""
        
        if [ -n "$WSL_DISTRO_NAME" ]; then
            current_distro="$WSL_DISTRO_NAME"
            
            # Find the line number of current distro in the list
            distro_number=$(echo "$wsl_distros" | grep -n "^$WSL_DISTRO_NAME$" | cut -d: -f1)
            
            # If not found, default to 1
            if [ -z "$distro_number" ]; then
                distro_number="1"
            fi
        fi

        # Return both name and number
        echo "$distro_number: $current_distro"
    fi
}

# Method 1: Using command substitution with both name and number
# echo "[\[\033[1;31m\]\$(distro_name)\[\033[0m\]]:\u@\h>\[\033[38;5;41m\]\W\[\033[0m\]\$"

# Method 2: If you want to use it in PS1
PS1="[\[\033[1;31m\]\$(distro_name)\[\033[0m\]]:\u@\h>\[\033[38;5;41m\]\W\[\033[0m\]\$"

# Method 3: More explicit formatting
PS1="[\[\033[1;31m\]\$(distro_name)\[\033[0m\]]:\u@\h>\[\033[38;5;41m\]\W\[\033[0m\]\$"

