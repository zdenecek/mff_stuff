<?php
$config = parse_ini_file(__DIR__ . '/../.env');
define('BASE_PATH', rtrim($config['BASE_URL'] ?? '', '/') );

function url($path = '') {
    // Remove leading slash if present to avoid double slashes
    $path = ltrim($path, '/');
    return BASE_PATH . '/' . $path;
} 