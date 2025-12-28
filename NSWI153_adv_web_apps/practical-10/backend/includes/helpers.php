<?php
// Try to load from .env if exists, otherwise use empty string
$config = file_exists(__DIR__ . '/../.env') ? parse_ini_file(__DIR__ . '/../.env') : [];
define('BASE_PATH', rtrim($config['BASE_URL'] ?? '', '/') );

function url($path = '') {
    // Remove leading slash if present to avoid double slashes
    $path = ltrim($path, '/');
    return BASE_PATH . '/' . $path;
} 