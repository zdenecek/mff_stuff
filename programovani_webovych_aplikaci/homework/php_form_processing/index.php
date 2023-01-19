<?php

require_once(__DIR__ . "/recodex_lib.php");

function timestr2mins(string $str): int
{
    $items = explode(":", $str);
    return intval($items[0]) * 60 + intval($items[1]);
}

function ValidateTimestr(string $timestr): bool
{
    $reg= "/^(?:2[0-3]|[01][0-9]|[0-9]):[0-5][0-9]$/";
    return preg_match($reg,$timestr);
}

function ValidateFirstName($data): bool
{
    return ! (empty($data) or strlen($data) > 100) ;
}
function ValidateLastName($data): bool
{
    return ! (empty($data) or strlen($data) > 100) ;
}
function ValidateEmail($data): bool
{
    return ! (empty($data) or strlen($data) > 200 or !filter_var($data, FILTER_VALIDATE_EMAIL)) ;
}
function ValidateDeliveryBoy($data): bool
{
    return ! (empty($data) or !(in_array($data, ["jesus", "santa", "moroz", "hogfather", "czpost", "fedex"]))) ;
}
function ValidateUnboxDay($data): bool
{
    return ! (empty($data) or !(in_array($data, ["24", "25"]))) ;
}
function ValidateAndParseTime($data)
{
    if (empty($data)) {
        return null;
    }
    if (ValidateTimestr($data)) {
        return timestr2mins($data);
    }
    return false;
}


function validateFormAndSave()
{
    $invalid = [];
    var_dump($_POST);


    $firstName = $_POST["firstName"];
    $lastName = $_POST["lastName"];
    $email = $_POST["email"];
    $deliveryBoy = $_POST["deliveryBoy"];
    $unboxDay = $_POST["unboxDay"];

    $fromTime = array_key_exists('fromTime', $_POST) ? ValidateAndParseTime($_POST["fromTime"]) : false;
    $toTime = array_key_exists('toTime', $_POST) ? ValidateAndParseTime($_POST["toTime"]) : false;

    if (!ValidateFirstName($firstName)) {
        $invalid[] = "firstName";
    }
    if (!ValidateLastName($lastName)) {
        $invalid[] = "lastName";
    }
    if (!ValidateEmail($email)) {
        $invalid[] = "email";
    }
    if (!ValidateDeliveryBoy($deliveryBoy)) {
        $invalid[] = "deliveryBoy";
    }
    if (!ValidateUnboxDay($unboxDay)) {
        $invalid[] = "unboxDay";
    }
    if ($fromTime === false) {
        $invalid[] = "fromTime";
    }
    if ($toTime === false) {
        $invalid[] = "toTime";
    }

    $giftCustom = null;

    if (!array_key_exists('gifts', $_POST)) {
        $gifts=[];
    } else {
        $gifts=$_POST['gifts'];

        $i = false;
        array_walk($gifts, function ($gift) use (&$i) {
            if (!in_array($gift, ["socks","points","jarnik","cash","teddy","other"])) {
                $i = true;
            }
        });
        if ($i) {
            $invalid[] = "gifts";
        }

        if (in_array('other', $gifts)) {
            $giftCustom = $_POST['giftCustom'];
            if (empty($giftCustom) or strlen($giftCustom)> 100) {
                $invalid[] = 'giftCustom';
            }
        }
    }

    if ($invalid) {
        recodex_survey_error("Invalid or corrupt data error.", $invalid);
    } else {
        recodex_save_survey($firstName, $lastName, $email, $deliveryBoy, (int)$unboxDay, $fromTime, $toTime, $gifts, $giftCustom);
    }
}

if ($_SERVER["REQUEST_METHOD"] === "POST") {
    validateFormAndSave();
    header("Location: " . $_SERVER["PHP_SELF"]);
    die;
} else {
    require __DIR__ . "/form_template.html";
}
