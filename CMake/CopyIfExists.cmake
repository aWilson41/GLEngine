# Copies _SRC to _DEST if _DEST exists
function (COPY_IF_EXISTS _SRC _DEST)
	IF (EXISTS ${CMAKE_INSTALL_PREFIX}/${_DEST})
		EXECUTE_PROCESS (COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${_SRC} ${CMAKE_INSTALL_PREFIX}/${_DEST})
	ENDIF (NOT EXISTS ${CMAKE_INSTALL_PREFIX}/${_DEST})
endfunction()