<?php
class Permissions{
	private $permissions;
	const PERM_PROFILE_ACCESS_PUBLIC = 1;
	const PERM_PROFILE_SHOWFRIENDS = 2;
	const PERM_PROFILE_SHOWLOGIN = 4;
	const PERM_PROFILE_SHOWPROFUPDATED = 8;
	const PERM_PROFILE_SHOWEMAIL = 16;
	const PERM_EVENT_PRIVATE = 32;
	const PERM_EVENT_PUBLIC = 64;
	const PERM_SAFEVIEW = 128;
	public function __construct($permissions = -1){
		if ($permissions === -1){
			//set default
			$permissions = self::PERM_EVENT_PUBLIC | self::PERM_PROFILE_SHOWFRIENDS  
				| self::PERM_PROFILE_SHOWLOGIN | self::PERM_PROFILE_SHOWPROFUPDATED
				| self::PERM_EVENT_PUBLIC | self::PERM_EVENT_PRIVATE | self::PERM_SAFEVIEW;
		}
	}
	/**simply return current permissions*/
	public function get(){
		return $this->permissions;
	}

}
?>