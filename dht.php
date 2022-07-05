<?php
class dht11{
 public $link='';
 function __construct($population){
  $this->connect();
  $this->storeInDB($population);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'metrostationinfo') or die('Cannot select the DB');
 }
 
 function storeInDB($population){
  $query = "insert into population set population='".$population."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['population'] != ''){
 $dht11=new dht11($_GET['population']);
}


?>