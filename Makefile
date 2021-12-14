agrobiodiversity_model: initialize
	g++ src/main.cpp -std=c++11 -I src/lib/bmp -Ofast -o agrobiodiversity_model.out

initialize:
	@if [ ! -e src/test ]; then\
		mkdir src/test;\
		mkdir src/test/plot;\
	fi
	@if [ ! -e src/lib ]; then\
		mkdir src/lib;\
	fi
	@if [ ! -e src/lib/bmp ]; then\
		wget -O src/lib/EasyBMP.zip http://prdownloads.sourceforge.net/easybmp/EasyBMP_1.06.zip?download;\
		mkdir bmp;\
		unzip src/lib/EasyBMP.zip -d src/lib/bmp;\
	fi

clean:
	rm -R src/lib
