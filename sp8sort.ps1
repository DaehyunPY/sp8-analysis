function nixpath {
    $resolved = Resolve-Path $args
    Write-Output "/$( $resolved.Path.Replace(':\', '/').Replace('\', '/') )".Replace('//', '/')
}

docker run -ti --rm -v HOST:DOCKER david9107/sp8-analysis sp8sort $(nixpath $args)
