#/bin/bash

export LC_ALL=C

DirName=$(dirname "$0")

ascii2uni="$DirName/../ascii2uni -q"
uni2ascii="$DirName/../uni2ascii -q"

ToHex() {
    for ((i=0; i<${#1}; ++i)); do
        printf '%02x' "'${1:$i:1}"
    done <<<"$1"
    printf '\n'
}

Test() {
    Conv="$1"
    From="$2"
    FromHex="$(ToHex "$From")"
    ExpHex="$3"
    ResHex=$(printf '%s' "$From" | $ascii2uni "-a$Conv" | od -tx1 | sed 's/^\S* //;s/ //g' | head -n-1)
    printf "conv=$Conv, '$FromHex' -> '$ResHex'"
    if [ "$ResHex" = "$ExpHex" ]; then
        echo " OK"
    elif [ "$ResHex" = "$ExpHex"'0a' ]; then
        echo " OK (other than adding \\n)"
    else
        echo " *** Bad Expected='$ExpHex'"
    fi
}

printf '\n  Simplest case (\\u and \\U) \n\n'
Test U $'\\u00e1'      'c3a1'
Test L $'\\U000000e1'  'c3a1'

printf '\n  Erroneously merged hexdigit\n\n'
Test U $'\\u00e1b'     'c3a162'
Test L $'\\U000000e1b' 'c3a162'

printf '\n  Testing \\U0001f354 (hamburger)\n\n'
Test U $'\\U0001f354'  'f09f8d94'
Test L $'\\U0001f354'  'f09f8d94'

# shouldn't generate CESU8 (unless explicitly asked to)
printf '\n  Testing \\ud83c\\udf54 (surrogate pair)\n\n'
Test U $'\\ud83c\\udf54' 'f09f8d94'
Test L $'\\ud83c\\udf54' 'f09f8d94'

exit
Result:
conv=U, *** Bad '\u00e1b' -> 'e0b89b', expected 'c3a162'
conv=L, *** Bad '\U000000e1b' -> 'e0b89b', expected 'c3a162'
