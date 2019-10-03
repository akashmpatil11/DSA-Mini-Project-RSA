# DSA-Mini-Project-RSA
## Name: PATIL AKASH M.
## MIS No: 111503075
## RSA Encryption using C
  This program encrypts/decrypts given text file using RSA
  asymmectric algorithm.

### About Project
  <ul>
    <li>RSA algorithm uses two big (100 digit) prime numbers to generate Public and Private key</li>
    <li>For computation GMP Library (GNU Multiple Precision Arithmetic Library) is used</li>
    <li>For genrating prime <em>Miller-Rabin algorithm</em> is used</li>
    <li>This Program takes input as text file from user and encrypts data and also saves Private Key in another file</li>
    <li>Using Private Key User can decrypt data again</li>
   </ul>

### Usage
  <ul>
      $ make
    <li>For Help</li>
      $ ./project -h<br>
     <li>For Encryption</li>
      $ ./project -e filename1 filename2 filename3<br>
     <li>For Decryption</li>
      $ ./project -d filename1 filename2 filename3<br>
  </ul>
