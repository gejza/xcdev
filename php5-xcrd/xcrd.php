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

$val = array(array('a' => 1, 'b' => 'text'), array('c' => 1, 'd' => 'text'));
var_dump($val);
$str = xcrd_serialize($val);
var_dump(xcrd_unserialize($str));

xcrd_load('test.cdb');
echo print_r(xcrd_lookup(1, 'skoly'),1)."|\n";

echo print_r(xcrd_lookup(1, 'text'),1)."|\n";
/*xcrd_load('test2.cdb');
echo xcrd_lookup(1, 'aaa')."|\n";
echo xcrd_lookup(1, 'bbb')."|\n";*/
?>
