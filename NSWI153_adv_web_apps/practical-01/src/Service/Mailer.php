<?php

namespace YourVendor\YourProject\Service;

use Psr\Log\LoggerInterface;

class Mailer
{
    private $logger;

    public function __construct(LoggerInterface $logger)
    {
        $this->logger = $logger;
    }

    public function send(string $to, string $content): void
    {
        try {
            // Simulate email sending
            // In a real application, this would send an actual email
            echo "Sending email to {$to}: {$content}\n";
            
            // Simulate random failure
            if (rand(0, 4) === 0) {
                throw new \Exception("Failed to send email");
            }
        } catch (\Exception $e) {
            $this->logger->error('Can not send an e-mail.', [
                'email' => $to,
                'error' => $e->getMessage()
            ]);
            throw $e;
        }
    }
}