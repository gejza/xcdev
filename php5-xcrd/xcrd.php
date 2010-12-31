<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('xcrd')) {
	dl('xcrd.' . PHP_SHLIB_SUFFIX);
}
$module = 'xcrd';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";

xcrd_load('test.cdb');
echo xcrd_lookup(1, 'aaa')."|\n";
xcrd_load('test.cdb1');
echo xcrd_lookup(1, 'aaa')."|\n";
echo xcrd_lookup(1, 'bbb')."|\n";
?>
