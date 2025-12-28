<?php

namespace App\Controller;

use App\Entity\Author;
use App\Repository\AuthorRepository;
use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;

class AuthorController
{
    private PhpRenderer $view;
    private AuthorRepository $authorRepository;

    public function __construct(PhpRenderer $view, AuthorRepository $authorRepository)
    {
        $this->view = $view;
        $this->authorRepository = $authorRepository;
    }

    public function list(Request $request, Response $response): Response
    {
        $authors = $this->authorRepository->findAll();
        return $this->view->render($response, 'authors/list.php', ['authors' => $authors]);
    }

    public function registerForm(Request $request, Response $response): Response
    {
        return $this->view->render($response, 'authors/register.php');
    }

    public function register(Request $request, Response $response): Response
    {
        $data = $request->getParsedBody();
        $name = $data['name'] ?? '';

        if (empty($name)) {
            return $this->view->render($response, 'authors/register.php', [
                'error' => 'Name is required'
            ]);
        }

        $author = new Author();
        $author->setName($name);
        $this->authorRepository->save($author);

        return $response->withHeader('Location', url('/authors'))->withStatus(302);
    }
} 