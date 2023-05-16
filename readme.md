# Lab 4 server test för windows
Här är en liten testserver för windows lab 4 i operativsystem. Den kollar lite lätt om det som den tar emot är i rätt format och svarar klienten med antingen "Well done! B-)\n" eller "Try again\n". Den skriver även ned information om vilka som har fått "godkänt" i en txt fil i res/ .

## Kompilera
Kompilera server:

```ps
mingw32-make.exe server
```

Kompilera klient:

```ps
mingw32-make.exe client
```

## Kör
Server:
```ps
./out/server
```
Klient:
```ps
./out/client
```
