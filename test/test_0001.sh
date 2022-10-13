#/bin/bash

export LC_ALL=C

DirName=$(dirname "$0")

ascii2uni="$DirName/../ascii2uni"
uni2ascii="$DirName/../uni2ascii"

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
    else
        echo " *** Bad Expected='$ExpHex'"
    fi
}

Test U $'\\u00e1\n'     'c3a10a'
Test L $'\\U000000e1\n' 'c3a10a'
Test U $'\\u00e1b'      'c3a162'
Test L $'\\U000000e1b'  'c3a162'

exit
Result:
conv=U, *** Bad '\u00e1b' -> 'e0b89b', expected 'c3a162'
conv=L, *** Bad '\U000000e1b' -> 'e0b89b', expected 'c3a162'
