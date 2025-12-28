<?php

use App\Controller\ArticleController;
use App\Controller\AuthorController;
use App\Controller\HomeController;
use Slim\App;

return function (App $app) {
    // Web routes
    $app->get('/', [HomeController::class, 'index']);
    $app->get('/authors', [AuthorController::class, 'list']);
    $app->get('/register-author', [AuthorController::class, 'registerForm']);
    $app->post('/register-author', [AuthorController::class, 'register']);
    $app->get('/articles', [ArticleController::class, 'list']);
    $app->get('/create-article', [ArticleController::class, 'createForm']);
    $app->post('/create-article', [ArticleController::class, 'create']);

    // API routes
    $app->get('/api/v1/article-detail/{id}', [ArticleController::class, 'detail']);
    $app->get('/api/v1/articles', [ArticleController::class, 'listApi']);
    $app->post('/api/v1/articles', [ArticleController::class, 'createApi']);
}; 