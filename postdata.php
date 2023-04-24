<?php
  //koneksi ke database
  $konek = mysqli_connect("localhost", "root", "", "websensor");

  //baca data yang dikirim oleh wemos
  $v_tagid = $_GET["tagid"];

  mysqli_query($konek, "UPDATE rfidtag SET tagid='$v_tagid'");

?>