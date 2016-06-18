function Get-CurrFolderName
{
    $s = $(Get-Location).Path.Split("\")
    return $s[$s.Length - 1]
}

Push-Location

while ($(Get-CurrFolderName) -ne "cnnr-engine")
{
    cd ..
}

if (Test-Path "bin\Win32\Debug") {
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\serialization.dll" -Destination "bin\Win32\Debug\serialization.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\imageload.dll" -Destination "bin\Win32\Debug\imageload.dll"
}

if (Test-Path "bin\Win32\Release") {
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\serialization.dll" -Destination "bin\Win32\Release\serialization.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\imageload.dll" -Destination "bin\Win32\Release\imageload.dll"
}

if (Test-Path "bin\x64\Debug") {
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\serialization.dll" -Destination "bin\x64\Debug\serialization.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\imageload.dll" -Destination "bin\x64\Debug\imageload.dll"
}

if (Test-Path "bin\x64\Release") {
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\serialization.dll" -Destination "bin\x64\Release\serialization.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\imageload.dll" -Destination "bin\x64\Release\imageload.dll"
}

Pop-Location
