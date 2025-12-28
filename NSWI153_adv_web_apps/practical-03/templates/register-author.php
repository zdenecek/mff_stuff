<?php
$content = <<<HTML
<h1>Register Author</h1>
<form method="POST" action="#">
    <div>
        <label for="name">Name:</label>
        <input type="text" id="name" name="name" required>
    </div>
    <button type="submit">Register</button>
</form>
HTML;
require __DIR__ . '/layout.php'; 