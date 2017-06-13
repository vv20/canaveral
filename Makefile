SRC_DIR = src/
BIN_DIR = bin/
EXEC_NAME = launchpad

${BIN_DIR}${EXEC_NAME} : ${SRC_DIR}${EXEC_NAME}
	if [ ! -d ${BIN_DIR} ]; then mkdir ${BIN_DIR}; fi
	mv ${SRC_DIR}${EXEC_NAME} ${BIN_DIR}${EXEC_NAME}

${SRC_DIR}${EXEC_NAME} :
	cd ${SRC_DIR}; make 

clean : 
	cd ${SRC_DIR}; make clean
