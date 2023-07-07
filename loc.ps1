$file_map = @{}

(git ls-files) | ForEach-Object {
    $extension = [System.IO.Path]::GetExtension($_)
    if ($extension) {
        $len = (Get-Content $_).Length
        if ($file_map.ContainsKey($extension)) {
            $file_map[$extension] = $file_map[$extension] + $len
        } else {
            $file_map[$extension] = $len
        }
    }
}

$file_map

$total = 0
$file_map.GetEnumerator() | ForEach-Object {
    $total = $total + $_.Value
}

Write-Host "Total: ----------------------- $total"
