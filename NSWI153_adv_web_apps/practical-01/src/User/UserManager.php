<?php

namespace YourVendor\YourProject\User;

use YourVendor\YourProject\Service\Mailer;
use Psr\Log\LoggerInterface;

class UserManager
{
    private $mailer;
    private $logger;

    public function __construct(Mailer $mailer, LoggerInterface $logger)
    {
        $this->mailer = $mailer;
        $this->logger = $logger;
    }

    public function getUsers(): array
    {
        return [
            'user1@example.com',
            'user2@example.com',
            'user3@example.com'
        ];
    }

    public function notifyUsers(string $content): void
    {
        $this->logger->info('Notifying users.');
        
        $users = $this->getUsers();
        foreach ($users as $userEmail) {
            $this->mailer->send($userEmail, $content);
        }
    }
}
