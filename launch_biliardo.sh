#!/bin/bash

# Imposta DISPLAY per X11
export DISPLAY=:0

# Controlla se VcXsrv è in esecuzione su Windows
# Usa tasklist via PowerShell
RUNNING=$(powershell.exe -Command "Get-Process vcxsrv -ErrorAction SilentlyContinue")

if [ -z "$RUNNING" ]; then
    echo "VcXsrv non è in esecuzione, lo avvio..."
    # Avvia VcXsrv con parametri base: multiple windows e no access control
    powershell.exe -Command "Start-Process 'C:\\Program Files\\VcXsrv\\vcxsrv.exe' -ArgumentList ':0 -multiwindow -ac' -WindowStyle Hidden"
    sleep 2  # aspetta 2 secondi che X server sia pronto
fi


