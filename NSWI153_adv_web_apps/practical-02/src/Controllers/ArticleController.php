<?php

namespace App\Controllers;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;




class ArticleController
{
    private $renderer;

    public function __construct(PhpRenderer $renderer)
    {
        $this->renderer = $renderer;
    }

    public function list(Request $request, Response $response): Response
    {
        $response->getBody()->write("Coming soon!");
        return $response->withStatus(501);
    }

    public function createForm(Request $request, Response $response): Response
    {
        $response->getBody()->write("Coming soon!");
        return $response->withStatus(501);
    }
} 