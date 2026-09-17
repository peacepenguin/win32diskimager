$exe = ".\dist\WinDiskImager.exe"

$cultures = @(
    "de-DE",
    "es-ES",
    "fr-FR",
    "it-IT",
    "ja-JP",
    "ko-KR",
    "nl-NL",
    "pl-PL",
    "ta-IN",
    "zh-CN",
    "zh-TW"
)

# Remember the original culture so we can restore it afterward
$originalCulture = (Get-Culture).Name

Write-Host "Original culture: $originalCulture" -ForegroundColor Cyan
Write-Host "Testing $($cultures.Count) cultures..." -ForegroundColor Cyan
Write-Host ""

try {
    foreach ($culture in $cultures) {
        Write-Host "========================================" -ForegroundColor Yellow
        Write-Host " Testing culture: $culture" -ForegroundColor Yellow
        Write-Host "========================================" -ForegroundColor Yellow

        # Set Windows culture
        Set-Culture -CultureInfo $culture

        Write-Host "Launching WinDiskImager..." -ForegroundColor Green

        # Launch application and wait until it exits
        Start-Process -FilePath $exe -Wait

        Write-Host "WinDiskImager closed." -ForegroundColor DarkGray
        Write-Host ""
    }
}
finally {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host " Testing complete" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan

    Write-Host "Restoring original culture: $originalCulture"
    Set-Culture -CultureInfo $originalCulture

    Write-Host "Done." -ForegroundColor Green
}