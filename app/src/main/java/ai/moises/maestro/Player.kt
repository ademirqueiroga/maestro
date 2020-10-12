package com.admqueiroga.maestro

/**
 * Created by Ademir Queiroga <ademir@fuze.cc> on 05/05/21.
 */
var pid: Long = 0L
class Player(val track: String) {
    val id: Long = ++pid
}