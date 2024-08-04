#!/bin/bash

# Wyodrębnij adresy IP do pliku ip.txt
netstat -tnlp | grep -oE '[0-9a-f:\.]+' | sed 's/:/. /g' > ip.txt

# Konwersja ip.txt na HTML
cat > netstat2.html << EOF
<!DOCTYPE html>
<html>
<head>
<title>Adresy IP z netstat</title>
</head>
<body>
<h1>Adresy IP z netstat</h1>
<ul>
EOF

while read ip; do
  # Sprawdź, czy adres IP jest IPv4
  if [[ $ip =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    # Adres IPv4 - wyświetl bez zmian
    echo "<li><a href=\"http://$ip\">$ip</a></li>" >> netstat2.html
  else
    # Adres IPv6 - przekonwertuj na IPv4
    ipv4=$(condense_ipv6 $ip)
    if [ -n "$ipv4" ]; then
      echo "<li><a href=\"http://$ipv4\">$ip ($ipv4)</a></li>" >> netstat2.html
    else
      # Nie udało się skrócić adresu IPv6 - pomiń go
      echo "<li>$ip (nie udało się skrócić do IPv4)</li>" >> netstat2.html
    fi
  fi
#done < ip.txt

#cat >> netstat2.html << EOF


while read ip; do
  echo "<li><a href=\"http://$ip\">$ip</a></li>" >> netstat.html
done < ip.txt

cat >> netstat.html << EOF

</ul>
</body>
</html>
EOF

# Usuń plik ip.txt
rm ip.txt

# Funkcja do skracania adresów IPv6 do IPv4
condense_ipv6() {
  # Sprawdź, czy adres IPv6 jest w formacie skróconym
  if [[ $1 =~ ^::([0-9a-f]+):([0-9a-f]+):([0-9a-f]+):([0-9a-f]+)$ ]]; then
    # Wyodrębnij części adresu
    part1=${BASH_REMATCH[1]}
    part2=${BASH_REMATCH[2]}
    part3=${BASH_REMATCH[3]}
    part4=${BASH_REMATCH[4]}

    # Skróć adres, jeśli to możliwe
    if [[ $part1 == 0000 ]]; then
      # Usuń początkowe zerowe części
      part1=""
    fi
    if [[ $part2 == 0000 ]]; then
      part2=""
    fi
    if [[ $part3 == 0000 ]]; then
      part3=""
    fi
    if [[ $part4 == 0000 ]]; then
      part4=""
    fi

    # Zrekonstruuj skrócony adres IPv4
    ipv4="$part1:$part2:$part3:$part4"
  else
    # Adres IPv6 nie jest w formacie skróconym - zwróć pusty ciąg
    ipv4=""
  fi

  echo $ipv4
}
