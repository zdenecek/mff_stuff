<?php
$content = "
<h1>Registration Successful</h1>
<p>" . htmlspecialchars($name) . ", welcome!</p>
";
require __DIR__ . '/layout.php'; 