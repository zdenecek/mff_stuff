<?php

require __DIR__ . '/vendor/autoload.php';

use App\Config\DoctrineConfig;

$entityManager = DoctrineConfig::getEntityManager();

$schemaTool = new \Doctrine\ORM\Tools\SchemaTool($entityManager);
$metadata = $entityManager->getMetadataFactory()->getAllMetadata();

$schemaTool->createSchema($metadata);

echo "Database schema created successfully!\n"; 