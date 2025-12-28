<?php

use App\Controllers\HomeController;
use App\Controllers\AuthorController;
use App\Controllers\ArticleController;
use Slim\App;

return function (App $app) {
    // Home page
    $app->get('/', [HomeController::class, 'index']);

    // Author routes
    $app->get('/authors', [AuthorController::class, 'list']);
    $app->get('/register-author', [AuthorController::class, 'registerForm']);
    $app->post('/register-author', [AuthorController::class, 'register']);

    // Article routes
    $app->get('/articles', [ArticleController::class, 'list']);
    $app->get('/create-article', [ArticleController::class, 'createForm']);
}; 