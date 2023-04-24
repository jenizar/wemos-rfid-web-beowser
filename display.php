<?php
  //koneksi ke database
  $konek = mysqli_connect("localhost", "root", "", "websensor");

  //baca isi table loadcell
  $sql = mysqli_query($konek, "SELECT * FROM rfidtag");
  $data = mysqli_fetch_array($sql);
  $nilai = $data["tagid"];

  echo $nilai;
 ?>