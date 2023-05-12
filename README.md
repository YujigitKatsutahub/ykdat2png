数値解析などのデータを gnuplot によりグラフ化するツール

ubuntu22.04ja では、  
sudo apt install build-essential gnuplot  
make  
./ykdat2dat bf.dat  

でグラフが表示され、エンターで  
ykdat2png.png  :key  
ykdat2pngn.png :nokey  
が作成される

---

no title で、正方形、epsの場合は、  
./ykdat2png -no -s -e bf.dat  
である

