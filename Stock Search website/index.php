<?php   

header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST');


if(isset($_GET["term"])){
    
    $array_one = array();
    $array_two = array();
    
        $company = $_GET["term"];
        $xmlstr = "http://dev.markitondemand.com/MODApis/Api/v2/Lookup/xml?input=".urlencode($company);
        $loadXML = simplexml_load_file($xmlstr);
        
        foreach($loadXML->children() as $nodes){
            $array_one["value"] = (string)$nodes->Symbol;
            $array_one["label"] = (string)$nodes->Symbol." - ".(string)$nodes->Name." ( ".(string)$nodes->Exchange." )";
            array_push($array_two, $array_one);
        }
    
    echo json_encode($array_two);
     //echo $array_two;   
}

else if(isset($_GET['bingajax'])){
    $companysearch = (isset($_GET['bingajax']) ? $_GET['bingajax'] : null);
    
                    // Replace this value with your account key
                    $accountKey = 'YIXpgiNhGKHo/yP3MrN9rfE8I3oLQEddgtdwkRQmEGU';
            
                    $bingURL = "https://api.datamarket.azure.com/Bing/Search/News?Query=%27".urlencode($_GET['bingajax'])."%27&\$format=json";
                    
                    $context = stream_context_create(array(
                        'http' => array(
                            'request_fulluri' => true,
                            'header'  => "Authorization: Basic " . base64_encode($accountKey . ":" . $accountKey)
                        )
                    ));
                    
                    $response = file_get_contents($bingURL, 0, $context);
                    
                    echo $response;
                    
                } 

else if(isset($_GET['newchart'])){
        $companychart = (isset($_GET['newchart']) ? $_GET['newchart'] : null);
       /* $chartJSON = file_get_contents("http://dev.markitondemand.com/MODApis/Api/v2/InteractiveChart/json?parameters={%22Normalized%22:false,%22NumberOfDays%22:1095,%22DataPeriod%22:%22Day%22,%22Elements%22[{:%22Symbol%22:%22".$_GET['newchart']."%22,%22Type%22:%22price%22,%22Params%22:[%22ohlc%22]}]}");*/
        $loadJSON = file_get_contents("http://dev.markitondemand.com/MODApis/Api/v2/InteractiveChart/json?parameters=%7b%22Normalized%22:false,%22NumberOfDays%22:1095,%22DataPeriod%22:%22Day%22,%22Elements%22:%5b%7b%22Symbol%22:%22".urlencode($_GET['newchart'])."%22,%22Type%22:%22price%22,%22Params%22:%5b%22ohlc%22%5d%7d%5d%7d");
 // echo json_encode($loadJSON);
echo $loadJSON;
    
}

else if(isset($_GET['favcompany'])){
        $favJSON = file_get_contents("http://dev.markitondemand.com/MODApis/Api/v2/Quote/json?symbol=".urlencode($_GET['favcompany']));
        //$loadJSON = json_decode($newJSON);

 // echo json_encode($loadJSON);
echo $favJSON;
}


else if(isset($_GET['localcompany'])){
        $localJSON = file_get_contents("http://dev.markitondemand.com/MODApis/Api/v2/Quote/json?symbol=".urlencode($_GET['localcompany']));
        //$loadJSON = json_decode($newJSON);

 // echo json_encode($loadJSON);
echo $localJSON;
}

else if($_SERVER["REQUEST_METHOD"]){
        $company = (isset($_GET['txtCompany']) ? $_GET['txtCompany'] : null);
        $newJSON = file_get_contents("http://dev.markitondemand.com/MODApis/Api/v2/Quote/json?symbol=".$company);
        //$loadJSON = json_decode($newJSON);

 // echo json_encode($loadJSON);
echo $newJSON;
}




        
?>