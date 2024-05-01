数値解析などのデータを gnuplot によりグラフ化するツール

ubuntu22.04ja では、  
sudo apt install build-essential gnuplot  
make  
./ykdat2png bf.dat  

でグラフが表示され、エンターで  
ykdat2png.png  :key  
ykdat2pngn.png :nokey  
が作成される

(注) 上書きされるので、png を保存する場合は、ファイル名を変更すること

---

no title で、正方形、epsの場合は、  
./ykdat2png -no -s -e bf.dat  
である

---

複数のファイルの連結したグラフもできる  
./ykdat2png bf20191103a.dat bf20191103b.dat


