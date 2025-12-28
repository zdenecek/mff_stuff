<?php

define('BASE_PATH', '/~78002598/nswi153/practical-02'); // Adjust this to your actual path

function url($path = '') {
    // Remove leading slash if present to avoid double slashes
    $path = ltrim($path, '/');
    return BASE_PATH . '/' . $path;
} 