<?php 

class fw_comm
{
	private $addr_ip;
	private $port;
	private $socket;
	/*
	参数$_addr_ip表示防火墙服务的ip地址，默认为本机
	参数$_port表示防火墙服务的监听端口，默认为6667
	*/
	function __construct($_addr_ip="127.0.0.1", $_port=6667) {
		$this->addr_ip = $_addr_ip;
       	$this->port = $_port;
       	$this->init();
		$this->conn_to_fw();
   }
   function __destruct(){
   		$this->close_conn();
   }
   public function open_firewall($user, $password){
   		// $cmd = "nohup pkill rinetd &";
   		// exec($cmd, $array, $ret);
   		// if($ret != 0){
   		// 	return 0;
   		// }
   		$cmd = "echo '".$user." ".$password."'|nohup /etc/init.d/db_FireWall start &";
   		exec($cmd, $array, $ret);
   		return $ret == 0;
   }
   public function close_firewall(){
   		// $cmd = "nohup /etc/init.d/db_FireWall stop &";
   		// exec($cmd, $array, $ret);
   		// if($ret != 0){
   		// 	return 0;
   		// }
   		$cmd = "nohup rinetd -c /etc/rinetd.conf &";
   		exec($cmd, $array, $ret);
   		return $ret == 0;
   }
   public function restart_firewall($user, $password){
   		close_firewall();
   		return open_firewall($user, $password);
   }

   public function query_firewall(){
   		$cmd = "ps -aux|grep 'db_FireWall'|grep -v 'grep'";
   		exec($cmd, $array, $ret);
   		return count($array)!=0;
   }
   public function read_config(){
   		$msg = "\1\0\0\0";
   		$msg .=chr(32);
   		return $this->comm_with_fw($msg);
   }
   public function write_config($config){
   		$msg = $this->int_to_string_by_bit(strlen($config)+1);
   		$msg .=chr(33);
   		$msg .= $config;
   		return $this->comm_with_fw($msg);
   }
   /*
	public function switch_mode($mode)
	参数$mode为true(转换为学习模式)
	或者false(过滤模式)
	仅返回是否成功，不返回错误信息
	*/
	public function switch_mode($mode)
	{
		
		$msg = "\2\0\0\0\1";
		
		if($mode == true)
		{
			$msg = $msg."\1";
		}
		
		else if ($mode == false)
		{
			$msg = $msg."\0";
		}
		return $this->comm_with_fw($msg);
	}
	/*
	查询当前是否是学习模式，1是，0不是
	*/
	public function query_mode()
	{
		$msg = "\2\0\0\0\1\2";
		return $this->comm_with_fw($msg);
	}
	/*
	切换学习模式，并且返回切换后的模式，1是学习模式，2不是
	*/
	public function switch_and_query()
	{
		$msg = "\2\0\0\0\1\4";
		return $this->comm_with_fw($msg);
	}
	public function parse($sql){
		return $this->update_a_rule($sql, 0, "", 0, "", 0, chr(2), 0);
	}

	public function insert_to_db($sql, $user, $is_all_user, $addr_ip, $is_all_addr_ip){
		return $this->update_a_rule($sql, $is_all_addr_ip, $addr_ip, $is_all_user, $user, 0, "\3", 0);
	}

	public function delete_from_db($sql, $user, $addr_ip){
		return $this->update_a_rule($sql, 0, $addr_ip, 0, $user, 0, "\4", 0);
	}
	public function remove_from_list($sql, $level, $user, $addr_ip, $whitch_list){
		return $this->update_a_rule($sql, 0, $addr_ip, 0, $user, $level, "\5", $whitch_list);
	}
	
	public function add_to_list($sql, $level, $user, $addr_ip, $whitch_list){
		return $this->update_a_rule($sql, 0, $addr_ip, 0, $user, $level, "\6", $whitch_list);
	}
	public function add_to_list2($sql, $level, $user, $is_all_user, $addr_ip, $is_all_addr_ip, $whitch_list){
		return $this->update_a_rule($sql, $is_all_user, $addr_ip, $is_all_addr_ip, $user, $level, chr(7), $whitch_list);
	}
	
	private function update_a_rule($sql, $is_all_addr_ip, $addr_ip, $is_all_user, $user, $level, $type, $flag)
	{
		$msg_len = strlen($sql) + 3 + 30 + 30;
		$msg = $this->int_to_string_by_bit($msg_len);
		$msg = $msg.$type;
		$level_flag = $level;
		if($is_all_user == true)
		{
			$level_flag = $level_flag | (1<<4);
		}
		if($is_all_addr_ip == true)
		{
			$level_flag = $level_flag | (1<<3);
		}
		$msg = $msg.chr($level_flag);
		$msg = $msg.chr($flag);
		while(strlen($user) < 30)
		{
			$user = $user."\0";
		}
		$msg = $msg.$user;
		while(strlen($addr_ip) < 30)
		{
			$addr_ip = $addr_ip."\0";
		}
		$msg = $msg.$addr_ip;
		$msg = $msg.$sql;
		return $this->comm_with_fw($msg);
	}

	private function comm_with_fw($msg)
	{
		// $this->init();
		// $this->conn_to_fw();
		$this->send_msg($msg, strlen($msg));
		$res = $this->recv_msg();
		if(ord($res) > 1){
			$data = array();
			for($i=0;$i<ord($res);$i++){
				array_push($data, $this->recv_data());
			}
			return $data;
		}
		//$this->close_conn();
		return $res == "\1";
	}

	private function conn_to_fw()
	{
		if(socket_connect($this->socket, $this->addr_ip, $this->port) == false)
		{
			echo("connect error!");
		}
	}

	private function send_msg($msg, $size)
	{
		if(socket_write($this->socket, $msg, $size) == false)
		{
			echo("send msg error!");
		}
	}
	private function recv_msg()
	{
		$msg = socket_read($this->socket, 1);
		return $msg;
	}

	private function recv_data()
	{
		$size = $this->string_to_int_by_bit(socket_read($this->socket, 4));
		return socket_read($this->socket, $size);
	}

	private function close_conn()
	{
		socket_close($this->socket);
	}
	private function do_nothing()
	{

	}

	private function init()
	{
		$this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
	}

	private function int_to_string_by_bit($size)
	{
		$res = "";
		$cnt = 4;
		while($cnt > 0)
		{
			$res = $res.chr($size%256);
			$size /= 256;
			$cnt --;
		}
		return $res;
	}

	private function string_to_int_by_bit($str){
		$size = 0;
		for($i=3;$i>=0;$i--){
			$size = $size*256 + ord($str[$i]);
			//echo $size;
			//echo "<br>";
		}
		return $size;
	}
}


//$client = new fw_comm("127.0.0.1",6667);
//echo $client->switch_mode(1);

//print_r($client->query_firewall());
//print_r($client->open_firewall("root", "123456"));
//print_r($client->close_firewall());
#echo $client->switch_mode(0);
//print_r($client->read_config());
//print($client->write_config('{"server_port":3306,"firewall_port":6666,"ui_comm_port":6667,"is_learning":1,"LOG_LEVEL":0,"db_user":null,"db_password":null,"db_name":"LL_firewall","time_out":60,"listen_queue_size":1024,"default_level":0,"test":1}'));

/*
//test parse
$res = $client->parse("select col from tab where id = \"121\";");
echo count($res);
echo "<br>";
for($i=0;$i<count($res);$i++){
	echo $res[$i];
	echo "<br>";
}
*/


//test insert
//for($i=0;$i<=10000;$i++)
//	echo $client->insert_to_db("select * from white_list where sql_cmd = \"\"", "root" , 1,"127.0.0.1", 1);


/*
//test delete
echo $client->delete_from_db("select * from white_list where sql_cmd = \"\"","root", "127.0.0.1")
*/

///*
//test add
//echo $client->add_to_list("select * from white_list where sql_cmd = \"\"", 0, "root", "127.0.0.1", 0);
//*/

///*
//test delete
//echo $client->remove_from_list("select * from white_list where sql_cmd = \"\"", 0, "root", "127.0.0.1", 0);
//*/

?>
