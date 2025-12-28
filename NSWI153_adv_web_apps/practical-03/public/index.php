<?php

use DI\ContainerBuilder;
use Slim\Factory\AppFactory;
use Slim\Views\PhpRenderer;
use App\Config\DoctrineConfig;

require __DIR__ . '/../vendor/autoload.php';

// include this one manually. We could use a class in src, but lets keep it simple stupid
require __DIR__ . '/../includes/helpers.php';

// Load environment variables
$config = parse_ini_file(__DIR__ . '/../.env');

// Create Container
$containerBuilder = new ContainerBuilder();
DoctrineConfig::configureContainer($containerBuilder);

// Set view renderer
$containerBuilder->addDefinitions([
    PhpRenderer::class => function () {
        return new PhpRenderer(__DIR__ . '/../templates');
    }
]);

$container = $containerBuilder->build();

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