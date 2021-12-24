$failure = 0

& "./scripts/mesh-test-compare.ps1"
$failure += $LASTEXITCODE

& "./scripts/transform-test-compare.ps1"
$failure += $LASTEXITCODE

& "./scripts/pipeline-test-compare.ps1"
$failure += $LASTEXITCODE

& "./scripts/texture-test-compare.ps1"
$failure += $LASTEXITCODE

& "./scripts/sampler-test-compare.ps1"
$failure += $LASTEXITCODE

& "./scripts/material-test-compare.ps1"
$failure += $LASTEXITCODE

& "./scripts/entity-test-compare.ps1"
$failure += $LASTEXITCODE

& "./scripts/renderer-test-compare.ps1"
$failure += $LASTEXITCODE

Write-Output ""
Write-Output "Overall Results"
if($failure -eq 0){
    Write-Output "SUCCESS: All outputs are correct"
}else{
    if($failure -eq 1){
        Write-Output "FAILURE: $failure output is incorrect"
    }else{
        Write-Output "FAILURE: $failure outputs are incorrect"
    }
}