<?php

use DI\Container;
use Monolog\Logger;
use Monolog\Handler\StreamHandler;
use Monolog\Formatter\HtmlFormatter;
use Monolog\Processor\WebProcessor;
use YourVendor\YourProject\User\UserManager;
use YourVendor\YourProject\Service\Mailer;
use Psr\Log\LoggerInterface;

require __DIR__ . '/vendor/autoload.php';

// Create log directory if it doesn't exist
if (!is_dir(__DIR__ . '/log')) {
    mkdir(__DIR__ . '/log', 0777, true);
}

// Create DI container
$container = new Container();

// Configure logger
$container->set(LoggerInterface::class, function() {
    $logger = new Logger('application');
    
    // Add handlers
    $logger->pushHandler(new StreamHandler(__DIR__ . '/log/application.log'));
    
    $htmlHandler = new StreamHandler(__DIR__ . '/log/warning.html', Logger::WARNING);
    $htmlHandler->setFormatter(new HtmlFormatter());
    $logger->pushHandler($htmlHandler);
    
    // Add web processor
    $logger->pushProcessor(new WebProcessor());
    
    return $logger;
});

// Configure mailer
$container->set(Mailer::class, function(Container $container) {
    return new Mailer($container->get(LoggerInterface::class));
});

// Configure user manager
$container->set(UserManager::class, function(Container $container) {
    return new UserManager(
        $container->get(Mailer::class),
        $container->get(LoggerInterface::class)
    );
});

// Use the application
try {
    $userManager = $container->get(UserManager::class);
    $userManager->notifyUsers('Hello world!');
} catch (\Exception $e) {
    echo "An error occurred: " . $e->getMessage() . "\n";
}