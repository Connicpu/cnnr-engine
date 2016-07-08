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
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\serialization.pdb" -Destination "bin\Win32\Debug\serialization.pdb"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\imageload.dll" -Destination "bin\Win32\Debug\imageload.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\imageload.pdb" -Destination "bin\Win32\Debug\imageload.pdb"
}

if (Test-Path "bin\Win32\Release") {
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\serialization.dll" -Destination "bin\Win32\Release\serialization.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\serialization.pdb" -Destination "bin\Win32\Release\serialization.pdb"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\imageload.dll" -Destination "bin\Win32\Release\imageload.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\imageload.pdb" -Destination "bin\Win32\Release\imageload.pdb"
}

if (Test-Path "bin\x64\Debug") {
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\serialization.dll" -Destination "bin\x64\Debug\serialization.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\serialization.pdb" -Destination "bin\x64\Debug\serialization.pdb"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\imageload.dll" -Destination "bin\x64\Debug\imageload.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\imageload.pdb" -Destination "bin\x64\Debug\imageload.pdb"
}

if (Test-Path "bin\x64\Release") {
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\serialization.dll" -Destination "bin\x64\Release\serialization.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\serialization.pdb" -Destination "bin\x64\Release\serialization.pdb"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\imageload.dll" -Destination "bin\x64\Release\imageload.dll"
    Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\imageload.pdb" -Destination "bin\x64\Release\imageload.pdb"
}

Pop-Location
