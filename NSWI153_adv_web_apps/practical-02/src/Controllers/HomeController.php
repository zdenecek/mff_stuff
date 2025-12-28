<?php

namespace App\Controllers;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;

class HomeController
{
    private $renderer;

    public function __construct(PhpRenderer $renderer)
    {
        $this->renderer = $renderer;
    }

    public function index(Request $request, Response $response): Response
    {
        $templatePath = __DIR__ . '/../../templates/home.php';
        return $this->renderer->render($response, $templatePath);
    }
} 