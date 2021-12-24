$files = @(
    "default-0.png",
    "default-1.png",
    "default-2.png",
    "default-3.png",
    "monkey-0.png",
    "monkey-1.png",
    "monkey-2.png",
    "monkey-3.png"
)

$req = "mesh-test/"

$expected = "expected/$req"
$output  = "screenshots/$req"
$errors = "errors/$req"

if(!(Test-Path $errors)) {
    New-Item -ItemType Directory -Path $errors
}

$success = 0

foreach ($file in $files) {
    Write-Output "Testing $file ..."
    & "./scripts/imgcmp" "$expected$file" "$output$file" -o "$errors$file" -t 0.01
    if($LASTEXITCODE -eq 0){
        $success += 1
    }
}

$total = $files.Length
Write-Output ""
Write-Output "Matches: $success/$total"
if($success -eq $total){
    Write-Output "SUCCESS: All outputs are correct"
	exit 0
}else{
    $failure = $total - $success
    if($failure -eq 1){
        Write-Output "FAILURE: $failure output is incorrect"
    }else{
        Write-Output "FAILURE: $failure outputs are incorrect"
    }
	exit $failure
}