<?php

use DI\Container;
use Slim\Factory\AppFactory;
use Slim\Views\PhpRenderer;


require __DIR__ . '/../vendor/autoload.php';

// include this one manually. We could use a class in src, but lets keep it simple stupid
require __DIR__ . '/../includes/helpers.php';

// Create Container
$container = new Container();

// Set view renderer
$container->set('renderer', function () {
    return new PhpRenderer(__DIR__ . '/../templates');
});

// Create App
AppFactory::setContainer($container);
$app = AppFactory::create();

$app->setBasePath(BASE_PATH);

// Add Error Middleware
$app->addErrorMiddleware(true, true, true);

// Add routes
$routes = require __DIR__ . '/../src/routes.php';
$routes($app);

$app->run(); 