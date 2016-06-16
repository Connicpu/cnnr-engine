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

Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\serialization.dll" -Destination "bin\Win32\Debug\serialization.dll"
Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\serialization.dll" -Destination "bin\Win32\Release\serialization.dll"
Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\serialization.dll" -Destination "bin\x64\Debug\serialization.dll"
Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\serialization.dll" -Destination "bin\x64\Release\serialization.dll"

Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\imageload.dll" -Destination "bin\Win32\Debug\imageload.dll"
Copy-Item -Path "$env:CONNORLIB_HOME\bin\x86\imageload.dll" -Destination "bin\Win32\Release\imageload.dll"
Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\imageload.dll" -Destination "bin\x64\Debug\imageload.dll"
Copy-Item -Path "$env:CONNORLIB_HOME\bin\x64\imageload.dll" -Destination "bin\x64\Release\imageload.dll"

Pop-Location
