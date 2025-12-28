<?php

namespace App\Controllers;

use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;


class AuthorController
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

    public function registerForm(Request $request, Response $response): Response
    {
        $TEMPLATE_PATH = __DIR__ . '/../../templates/';
        return $this->renderer->render($response, $TEMPLATE_PATH . 'register-author.php');
    }

    public function register(Request $request, Response $response): Response
    {
        $data = $request->getParsedBody();
        $name = $data['name'] ?? '';
$TEMPLATE_PATH = __DIR__ . '/../../templates/';
       
       
        return $this->renderer->render($response, $TEMPLATE_PATH . 'register-success.php', [
            'name' => $name
        ]);
    }
} 