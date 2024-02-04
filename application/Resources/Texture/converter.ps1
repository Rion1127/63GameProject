$files = Get-Item *.png

foreach ($f in $files)
{
    # ミップレベル
    $mipLevel = 2  

    $arguments = @(
        $f.FullName,
        "-ml", $mipLevel
        # 他の引数を追加できます
    )

    Start-Process -FilePath "TextureConverter.exe" -ArgumentList $arguments -Wait
}

Pause