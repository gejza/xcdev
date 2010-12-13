<?php
# $Id: $
# File name: test_cb.php
# Date:      2010/12/06 20:46
# Author:    

$extension = "registry";
$extension_soname = $extension . "." . PHP_SHLIB_SUFFIX;
$extension_fullname = PHP_EXTENSION_DIR . "/" . $extension_soname;

// load extension
if (!extension_loaded($extension)) {
     dl($extension_soname) or die("Can't load extension $extension_fullname\n");
}

/*echo cb_string(1);
echo cb_string(2);
echo cb_string(3);
echo cb_string(4);*/
var_dump(registry_get());
