# SQL Manager 2010 Lite for MySQL 4.5.1.3
# ---------------------------------------
# Host     : localhost
# Port     : 3306
# Database : sincros


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES latin1 */;

SET FOREIGN_KEY_CHECKS=0;

CREATE DATABASE `sincros`
    CHARACTER SET 'latin1'
    COLLATE 'latin1_swedish_ci';

USE `sincros`;

#
# Structure for the `calibracion_encoder` table : 
#

CREATE TABLE `calibracion_encoder` (
  `id_calibracion_Enc` int(11) NOT NULL AUTO_INCREMENT,
  `vel_entrada` int(11) DEFAULT NULL COMMENT 'RPMs de entrada para encoder rango de 0-1000',
  `vel_encoder` int(11) DEFAULT NULL COMMENT 'Velocidad medida por el encoder (medicion real)',
  `vel_referencia` int(11) DEFAULT NULL COMMENT 'Velocidd de referencia para calibrar encoder, obtenida por instrumento de referencia por el usuario\n',
  `num_velocidad` int(11) DEFAULT NULL,
  `num_segmento` int(11) DEFAULT NULL COMMENT 'Número de segmento util para la linealizacion, el usuario podra definir tantos segmentos como sea necesario para realizar la calibracion\n',
  PRIMARY KEY (`id_calibracion_Enc`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Datos necesarios para la calibración encoder';

#
# Structure for the `transmisiones` table : 
#

CREATE TABLE `transmisiones` (
  `id_transmisiones` int(11) NOT NULL AUTO_INCREMENT,
  `identificador` varchar(250) DEFAULT ' ',
  `lado` int(11) NOT NULL DEFAULT '0' COMMENT '0 = Lado Izquierdo\r\n1 = Lado Derecho',
  `no_velocidades` int(11) DEFAULT '5' COMMENT 'numero de velocidades en transmision agregar N y R',
  PRIMARY KEY (`id_transmisiones`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

#
# Structure for the `config_velocidades` table : 
#

CREATE TABLE `config_velocidades` (
  `id_config_vel` int(11) NOT NULL AUTO_INCREMENT,
  `fk_id_transmisiones` int(11) NOT NULL,
  `num_velocidad` varchar(5) DEFAULT NULL COMMENT 'Identificador de la velocidad a configurar, Ej. 1 2 3 4 5 6 7 R N',
  `posicion_x` int(11) DEFAULT NULL COMMENT 'Posicion X correspondiente al plano de configuracion\n',
  `posicion_y` int(11) DEFAULT NULL COMMENT 'Posicion Y correspondiente al plano de configuracion',
  `plano` int(11) DEFAULT NULL COMMENT 'Plano correspondiente en base a las velocidades existentes en Horizontal.Auxiliar para calculo de ciclos',
  `tipo_vel` int(11) NOT NULL DEFAULT '0' COMMENT '-1 = Reversa\r\n0 = Neutral\r\n1 = Velocidad ',
  `velocidad` int(11) DEFAULT '0' COMMENT 'Valor numerico para la velocidad',
  PRIMARY KEY (`id_config_vel`,`fk_id_transmisiones`),
  KEY `fk_config_velocidades_transmisiones1` (`fk_id_transmisiones`),
  CONSTRAINT `fk_config_velocidades_transmisiones1` FOREIGN KEY (`fk_id_transmisiones`) REFERENCES `transmisiones` (`id_transmisiones`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='velocidades configuradas en cuanto a posicion fisica';

#
# Structure for the `contadores` table : 
#

CREATE TABLE `contadores` (
  `id_contador` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Identificado de la tabla de contadores',
  `fk_id_pruebag` int(11) NOT NULL DEFAULT '0' COMMENT 'Llave foranea para ligar la tabla con la de prueba_gral',
  `cont_ciclos` int(11) NOT NULL COMMENT 'Contador de ciclos, utili cuando se pusa y se reanida la prueba',
  `cont_choque` int(11) NOT NULL DEFAULT '1' COMMENT 'Contaor de choques general',
  `vel1` int(11) NOT NULL DEFAULT '0' COMMENT 'Variable para contar los choques realizados en esta etapa\r\n',
  `vel2` int(11) NOT NULL DEFAULT '0',
  `vel3` int(11) NOT NULL DEFAULT '0',
  `vel4` int(11) NOT NULL DEFAULT '0',
  `vel5` int(11) NOT NULL DEFAULT '0',
  `vel6` int(11) NOT NULL DEFAULT '0',
  `vel7` int(11) NOT NULL DEFAULT '0',
  `vel8` int(11) NOT NULL DEFAULT '0',
  `vel9` int(11) NOT NULL DEFAULT '0',
  `vel10` int(11) NOT NULL DEFAULT '0',
  `vel0` int(11) NOT NULL DEFAULT '0' COMMENT 'Variable para contar los choques realizados en la REVERSA',
  PRIMARY KEY (`id_contador`),
  UNIQUE KEY `id_contador` (`id_contador`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

#
# Structure for the `daq_entradas` table : 
#

CREATE TABLE `daq_entradas` (
  `id_daq_entradas` int(11) unsigned zerofill NOT NULL COMMENT 'Identificador de entrada',
  `canal` varchar(20) NOT NULL DEFAULT ' ' COMMENT 'Referencia del canal analogico, ejemplo Dev1/ai0',
  `panel` int(11) unsigned zerofill NOT NULL DEFAULT '00000000000' COMMENT 'Identificaror de panel',
  `descripcion` varchar(120) NOT NULL DEFAULT ' ' COMMENT 'Titulo del panel para la calibracion',
  `min` double(15,3) DEFAULT '0.000' COMMENT 'Valor minimo de canal en unidad de ingenieria',
  `max` double(15,3) DEFAULT '10.000' COMMENT 'Valor maximo de canal en unidad de ingenieria',
  `minescala` double(15,3) DEFAULT '0.000' COMMENT 'Valor minimo para la escala',
  `maxescala` double(15,3) DEFAULT '10.000' COMMENT 'Valor maximo para la escala',
  `unidad` varchar(20) DEFAULT ' ' COMMENT 'Unidad de medida "rpms, Nm", etc.',
  `identificador` int(11) DEFAULT '0' COMMENT 'Util para identificar el tipo de variable que es para diferenciar entre temperatura1, temperatura2 etc.',
  `orden` int(11) DEFAULT '0' COMMENT 'Variable que permite identificar en que orden de lectura analogica se encuentra el canal',
  PRIMARY KEY (`id_daq_entradas`),
  UNIQUE KEY `id_daq_entradas` (`id_daq_entradas`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Guarda las entradas, salidas analogicas y digitales';

#
# Structure for the `secuencias` table : 
#

CREATE TABLE `secuencias` (
  `id_secuencias` int(11) NOT NULL AUTO_INCREMENT,
  `fk_id_transmisiones` int(11) NOT NULL DEFAULT '0',
  `nombre_secuencia` varchar(45) DEFAULT NULL COMMENT 'Nombre de la secuencia',
  PRIMARY KEY (`id_secuencias`,`fk_id_transmisiones`),
  KEY `fk_secuencias_transmisiones1` (`fk_id_transmisiones`),
  CONSTRAINT `fk_secuencias_transmisiones1` FOREIGN KEY (`fk_id_transmisiones`) REFERENCES `transmisiones` (`id_transmisiones`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;

#
# Structure for the `tiempo_captura` table : 
#

CREATE TABLE `tiempo_captura` (
  `id_tiempo_captura` int(11) NOT NULL AUTO_INCREMENT,
  `tipo_captura` int(11) DEFAULT NULL COMMENT '0 = Ciclos definido\r\n1 = Tiempo definido\r\n2 = Lista de ciclos',
  `tiempo_captura` float DEFAULT NULL COMMENT 'Tiempo de captura minutos/horas/',
  `ciclos_def` int(11) DEFAULT NULL COMMENT 'Ciclos definidos por el usuario, Ej.- Cada 5000 ciclos se realizara la captura de datos',
  PRIMARY KEY (`id_tiempo_captura`)
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=latin1;

#
# Structure for the `prueba_gral` table : 
#

CREATE TABLE `prueba_gral` (
  `id_pruebag` int(11) NOT NULL AUTO_INCREMENT,
  `fk_id_tiempo_captura` int(11) NOT NULL COMMENT 'Periodo de adquisicion de datos',
  `fk_id_usuario` int(11) NOT NULL DEFAULT '0',
  `fk_id_secuencias` int(11) NOT NULL DEFAULT '0',
  `folio` varchar(45) DEFAULT NULL COMMENT 'Folio correspondiente para el registro de informacion',
  `descripcion` varchar(255) DEFAULT NULL COMMENT 'Nombre para la prueba',
  `validacion_celda` int(11) DEFAULT '0' COMMENT 'Se realizo la validacion de celda de carga\n1 = Validado\n0 = No Validado',
  `edo_validacion` int(11) DEFAULT NULL COMMENT 'Respuesta de usuario a medicion de fuerza\n1 = Aceptado\n0 = Rechazado',
  `tdms_datos` varchar(255) DEFAULT NULL COMMENT 'Ruta del tdms en el cual se almacena la captura de todas las variables del sistema',
  `tdms_calibracion` varchar(255) DEFAULT NULL COMMENT 'Ruta del archivo tdms donde se guardan los parametros utilizados para las diferentes calibraciones hechas durante la prueba',
  `edo_prueba` int(11) DEFAULT '0' COMMENT 'Estado de prueba\n0 = No ejecutada\n1 = En ejecucion\n2 = Pausada\n3 = Cancelada',
  `cont_prueba` int(11) DEFAULT '1' COMMENT 'Contador que nos permite diferencias cuantas calibraciones se ha hecho a una prueba, consecutivo ayuda a realizar select',
  PRIMARY KEY (`id_pruebag`,`fk_id_tiempo_captura`,`fk_id_usuario`,`fk_id_secuencias`),
  KEY `fk_prueba_gral_tiempo_captura1` (`fk_id_tiempo_captura`),
  KEY `fk_prueba_gral_usuarios1` (`fk_id_usuario`),
  KEY `fk_prueba_gral_secuencias1` (`fk_id_secuencias`),
  CONSTRAINT `fk_prueba_gral_secuencias1` FOREIGN KEY (`fk_id_secuencias`) REFERENCES `secuencias` (`id_secuencias`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_prueba_gral_tiempo_captura1` FOREIGN KEY (`fk_id_tiempo_captura`) REFERENCES `tiempo_captura` (`id_tiempo_captura`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1 COMMENT='configuracion de varios ciclos de sincronizacion\n';

#
# Structure for the `datos_fuerzap` table : 
#

CREATE TABLE `datos_fuerzap` (
  `id_datos_fuerzap` int(11) NOT NULL AUTO_INCREMENT,
  `fk_id_pruebag` int(11) NOT NULL DEFAULT '0' COMMENT 'FK de tabla prueba_grak',
  `longitud_shifter` double DEFAULT NULL COMMENT 'Es la longitud que tiene el shifter\n',
  `fuerza_esperada` double DEFAULT NULL,
  `ventaja_mecanica` double DEFAULT NULL COMMENT 'Ventaja mecanica = X / Y',
  `fuerza_perilla` double DEFAULT NULL COMMENT 'Fuerza perilla = Fuerza collarin / Ventaje mecanica\n',
  `fuerza_collarin` double DEFAULT NULL,
  `inercia` double(15,3) NOT NULL DEFAULT '0.000' COMMENT 'Valor de inercia para ejecutar movimientos',
  PRIMARY KEY (`id_datos_fuerzap`),
  KEY `fk_id_pruebag` (`fk_id_pruebag`),
  CONSTRAINT `datos_fuerzap_fk` FOREIGN KEY (`fk_id_pruebag`) REFERENCES `prueba_gral` (`id_pruebag`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Datos que permiten calcular la fuerza ejercida a la perilla\n';

#
# Structure for the `lista_ciclos` table : 
#

CREATE TABLE `lista_ciclos` (
  `id_lista_ciclos` int(11) NOT NULL AUTO_INCREMENT,
  `fk_id_tiempo_captura` int(11) NOT NULL DEFAULT '0',
  `ciclos_captura` int(11) DEFAULT NULL COMMENT 'Numero de ciclos en los cuales se iniciara la captura de datos',
  PRIMARY KEY (`id_lista_ciclos`,`fk_id_tiempo_captura`),
  KEY `fk_lista_ciclos_tiempo_captura` (`fk_id_tiempo_captura`),
  CONSTRAINT `fk_lista_ciclos_tiempo_captura` FOREIGN KEY (`fk_id_tiempo_captura`) REFERENCES `tiempo_captura` (`id_tiempo_captura`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=latin1 COMMENT='lista de ciclos';

#
# Structure for the `lista_pcalibracion` table : 
#

CREATE TABLE `lista_pcalibracion` (
  `id_pcalibracion` int(11) NOT NULL AUTO_INCREMENT,
  `fk_daq_entradas` int(11) NOT NULL DEFAULT '0' COMMENT 'Llave foranea de la tabla daq_entradas, permite extraer informacion del canal analogico',
  `vreal` double(15,3) NOT NULL DEFAULT '0.000' COMMENT 'Valor real adquirido',
  `vajustado` double(15,3) NOT NULL DEFAULT '0.000' COMMENT 'Valor ajustado, util para la configuracion de la escala en la task del canal',
  `voltaje` double(15,3) DEFAULT NULL COMMENT 'Util para realizar los ajustes en grafica del valor real y el ajustado',
  `fecha` datetime DEFAULT NULL COMMENT 'Fecha en la cual se agregaron los puntos de calibracion',
  `comentarios` varchar(255) DEFAULT ' ' COMMENT 'Comentario referente a la calibacion',
  PRIMARY KEY (`id_pcalibracion`),
  UNIQUE KEY `id_pcalibracion` (`id_pcalibracion`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

#
# Structure for the `niveles_mm` table : 
#

CREATE TABLE `niveles_mm` (
  `id_niveles` int(11) NOT NULL AUTO_INCREMENT,
  `vel_entrada` double DEFAULT NULL COMMENT '	',
  `vel_salida` double DEFAULT NULL,
  `torque_continuo` double DEFAULT NULL,
  `torque_pico` double DEFAULT NULL,
  `temp_max` double DEFAULT NULL COMMENT 'Limite de temperatura maxima del sistema',
  `temp_min` double DEFAULT NULL COMMENT 'Temperatura minima permitida para el sistema',
  `flujo_presion_max` double DEFAULT NULL COMMENT 'Flujo o Presion maximo permitido en el sistema',
  `flujo_presion_min` double DEFAULT NULL COMMENT 'Flujo o Presion minimo permitido en el sistema',
  `fuerza_max` double DEFAULT NULL COMMENT 'Fuerza maxima para el sistema\n',
  `fuerza_min` double DEFAULT NULL COMMENT 'Fuerza minima aplicada al sistema\n',
  `vibracion_max` double DEFAULT NULL,
  `vibracion_min` double DEFAULT NULL,
  `rpms_max` double DEFAULT NULL COMMENT 'Cantidad maxima de rpms permitida',
  `fuerzaperilla_max` double DEFAULT NULL COMMENT 'Fuerza maxima aplicada a la perilla y calculada por el software\n',
  PRIMARY KEY (`id_niveles`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1 COMMENT='Niveles maximos y minimos del sistema';

#
# Structure for the `pcalculos` table : 
#

CREATE TABLE `pcalculos` (
  `id_pCalculos` int(11) NOT NULL AUTO_INCREMENT,
  `iSampleRate` int(11) DEFAULT NULL,
  `iNumMuestras` int(11) DEFAULT NULL,
  `iCanalesAna` int(11) DEFAULT NULL COMMENT 'Total de canales analogicos\n',
  `iSampleRateWrite` int(11) DEFAULT NULL,
  `dRadio` double DEFAULT NULL,
  `dGravedad` double DEFAULT NULL,
  `dKgcms2ToLbins2` double DEFAULT NULL,
  `dRpmToRadseg2` double DEFAULT NULL,
  `dInerciaMotor` double DEFAULT NULL,
  `dTf` double DEFAULT NULL,
  `fCambioUnidades` int(11) DEFAULT NULL,
  PRIMARY KEY (`id_pCalculos`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

#
# Structure for the `pdimension` table : 
#

CREATE TABLE `pdimension` (
  `id_pDimension` int(11) NOT NULL AUTO_INCREMENT,
  `fLbtoAmp` float DEFAULT NULL,
  `iEje` int(11) DEFAULT NULL,
  `iDivisionesY` int(11) DEFAULT NULL,
  `iVariables` int(11) DEFAULT NULL,
  `iSegundos` int(11) DEFAULT NULL,
  `iSizeMsj` int(11) DEFAULT NULL,
  `iCantItems` int(11) DEFAULT NULL,
  `iSamplesPlot` int(11) DEFAULT NULL,
  PRIMARY KEY (`id_pDimension`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

#
# Structure for the `pescalas` table : 
#

CREATE TABLE `pescalas` (
  `id_pescalas` int(11) NOT NULL AUTO_INCREMENT,
  `dslope_Torque` double DEFAULT NULL,
  `dy0_torque` double DEFAULT NULL,
  `dslope_Nivel` double DEFAULT NULL,
  `dy0_nivel` double DEFAULT NULL,
  `dslop_Temp` double DEFAULT NULL,
  `dy0_temp` double DEFAULT NULL,
  `dslop_Flujo` double DEFAULT NULL,
  `dy0_flujo` double DEFAULT NULL,
  `dslope_Corriente` double DEFAULT NULL,
  `dy0_corriente` double DEFAULT NULL,
  PRIMARY KEY (`id_pescalas`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

#
# Structure for the `philos_motion` table : 
#

CREATE TABLE `philos_motion` (
  `id_philos_Motion` int(11) NOT NULL AUTO_INCREMENT,
  `iTarjetaID` int(11) DEFAULT NULL,
  `iEje` int(11) DEFAULT NULL,
  `iCuentasRev` int(11) DEFAULT NULL,
  `iTimeout` int(11) DEFAULT NULL,
  `iMotionSample` int(11) DEFAULT NULL,
  PRIMARY KEY (`id_philos_Motion`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

#
# Structure for the `pvalores` table : 
#

CREATE TABLE `pvalores` (
  `id_pValores` int(11) NOT NULL AUTO_INCREMENT,
  `dMaxSalAnaVolts` double DEFAULT NULL,
  `dMaxEntAna0` double DEFAULT NULL,
  `dMinEntAna0` double DEFAULT NULL,
  `dMaxEntAna1` double DEFAULT NULL,
  `dMinEntAna1` double DEFAULT NULL,
  `dMaxEntAna2` double DEFAULT NULL,
  `dMinEntAna2` double DEFAULT NULL,
  `dMaxEntAna3` double DEFAULT NULL,
  `dMinEntAna3` double DEFAULT NULL,
  `dMaxEntAna4` double DEFAULT NULL,
  `dMinEntAna4` double DEFAULT NULL,
  `dMaxEntAna5` double DEFAULT NULL,
  `dMinEntAna5` double DEFAULT NULL,
  `dMaxEntAna6` double DEFAULT NULL,
  `dMinEntAna6` double DEFAULT NULL,
  `dMaxEntAna7` double DEFAULT NULL,
  `dMinEntAna7` double DEFAULT NULL,
  PRIMARY KEY (`id_pValores`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

#
# Structure for the `registros` table : 
#

CREATE TABLE `registros` (
  `id_registro` int(11) NOT NULL AUTO_INCREMENT,
  `fk_id_pruebag` int(11) NOT NULL,
  `cont_ciclos` int(11) DEFAULT NULL COMMENT 'Contador de ciclos totales por prueba',
  PRIMARY KEY (`id_registro`,`fk_id_pruebag`),
  KEY `fk_registros_prueba_gral1` (`fk_id_pruebag`),
  CONSTRAINT `fk_registros_prueba_gral1` FOREIGN KEY (`fk_id_pruebag`) REFERENCES `prueba_gral` (`id_pruebag`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

#
# Structure for the `tipo_secuencia` table : 
#

CREATE TABLE `tipo_secuencia` (
  `id_tipo_sec` int(11) NOT NULL AUTO_INCREMENT,
  `fk_id_secuencias` int(11) NOT NULL,
  `no_secuencia` int(11) DEFAULT NULL COMMENT 'Número de movimiento de la secuencia',
  `pos_inicial` varchar(5) DEFAULT NULL COMMENT 'Posicion Inicial\nN1, 1, N2, 2 etc',
  `pos_final` varchar(5) DEFAULT NULL COMMENT 'Posicion Final 1, 2, N2, 2 etc',
  `descripcion` varchar(45) DEFAULT NULL COMMENT 'Descripcion de la secuencia de prueba ej.\n1 ---> 2 ---> 1\nR ---> N ---> R',
  `cicloxmin` int(11) DEFAULT NULL COMMENT 'Calculo de movimientos por minuto en base a los movimientos a realizar\n1-2-1 = 4\n1-5-1 = 8',
  `ciclostotal` int(11) DEFAULT NULL COMMENT 'Total de ciclos que se ejecutara la secuencia. Ej 50,000\n35,000.',
  `velocidadm` int(11) DEFAULT NULL COMMENT 'Velocidad de entrada para la secuencia en RPMs\n',
  PRIMARY KEY (`id_tipo_sec`,`fk_id_secuencias`),
  KEY `fk_tipo_secuencia_secuencias1` (`fk_id_secuencias`),
  CONSTRAINT `fk_tipo_secuencia_secuencias1` FOREIGN KEY (`fk_id_secuencias`) REFERENCES `secuencias` (`id_secuencias`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=latin1 COMMENT='desplazamiento entre posiciones de velocidad';

#
# Structure for the `tiempo_mov` table : 
#

CREATE TABLE `tiempo_mov` (
  `id_tiempo_mov` int(11) NOT NULL AUTO_INCREMENT,
  `fk_id_tipo_ciclo` int(11) NOT NULL,
  `num_movimiento` int(11) DEFAULT NULL COMMENT 'Numero de movimiento para desembragar velocidad',
  `pos_inicial` varchar(5) DEFAULT NULL COMMENT 'Posicion inicial de la palanca',
  `pos_final` varchar(5) DEFAULT NULL COMMENT 'Posicion final de la palanca',
  `tiempo` float DEFAULT NULL COMMENT 'Tiempo de ejecucion para realizar el movimiento de la palanca',
  PRIMARY KEY (`id_tiempo_mov`,`fk_id_tipo_ciclo`),
  KEY `fk_tiempo_mov_tipos_ciclo1` (`fk_id_tipo_ciclo`),
  CONSTRAINT `fk_tiempo_mov_tipos_ciclo1` FOREIGN KEY (`fk_id_tipo_ciclo`) REFERENCES `tipo_secuencia` (`id_tipo_sec`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='movimientos para secuencia con tiempo de ejecucion';

#
# Structure for the `usuarios` table : 
#

CREATE TABLE `usuarios` (
  `id_usuario` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(150) DEFAULT NULL,
  `tipo_usuario` int(11) DEFAULT NULL,
  `password` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id_usuario`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

#
# Data for the `transmisiones` table  (LIMIT 0,500)
#

INSERT INTO `transmisiones` (`id_transmisiones`, `identificador`, `lado`, `no_velocidades`) VALUES 
  (2,'Mustang GT',1,6),
  (5,'Mustang GT2',0,8),
  (6,'Corvette ZR1',0,7);
COMMIT;

#
# Data for the `daq_entradas` table  (LIMIT 0,500)
#

INSERT INTO `daq_entradas` (`id_daq_entradas`, `canal`, `panel`, `descripcion`, `min`, `max`, `minescala`, `maxescala`, `unidad`, `identificador`, `orden`) VALUES 
  (00000000000,' ',00000000000,' ',0.000,10.000,0.000,10.000,' ',0,0);
COMMIT;

#
# Data for the `secuencias` table  (LIMIT 0,500)
#

INSERT INTO `secuencias` (`id_secuencias`, `fk_id_transmisiones`, `nombre_secuencia`) VALUES 
  (5,2,'Prueba004'),
  (6,5,'Secuencia 4'),
  (7,6,'Secuencia 5');
COMMIT;

#
# Data for the `tiempo_captura` table  (LIMIT 0,500)
#

INSERT INTO `tiempo_captura` (`id_tiempo_captura`, `tipo_captura`, `tiempo_captura`, `ciclos_def`) VALUES 
  (1,0,0,2500),
  (2,0,0,2500),
  (3,0,0,3000),
  (4,0,0,3000),
  (5,0,0,3400),
  (6,0,0,3400),
  (7,0,0,2344),
  (8,0,0,2344),
  (9,0,0,3400),
  (10,0,0,3400),
  (11,0,0,2300),
  (12,0,0,2300),
  (13,0,0,1350),
  (14,1,260,0),
  (15,0,0,4566),
  (16,0,0,4566),
  (17,2,0,0),
  (18,2,0,0),
  (19,2,0,0),
  (20,2,0,0),
  (21,2,0,0),
  (22,2,0,0),
  (23,2,0,0),
  (24,2,0,0),
  (25,2,0,0),
  (26,2,0,0),
  (27,2,0,0);
COMMIT;

#
# Data for the `prueba_gral` table  (LIMIT 0,500)
#

INSERT INTO `prueba_gral` (`id_pruebag`, `fk_id_tiempo_captura`, `fk_id_usuario`, `fk_id_secuencias`, `folio`, `descripcion`, `validacion_celda`, `edo_validacion`, `tdms_datos`, `tdms_calibracion`, `edo_prueba`, `cont_prueba`) VALUES 
  (2,16,0,5,'Prueba 001','hola mundo',1,2,'','',-2,1),
  (4,14,0,6,'Prueba 002','Prueba demo',1,2,'','',-2,1);
COMMIT;

#
# Data for the `lista_ciclos` table  (LIMIT 0,500)
#

INSERT INTO `lista_ciclos` (`id_lista_ciclos`, `fk_id_tiempo_captura`, `ciclos_captura`) VALUES 
  (1,25,500),
  (2,25,500),
  (3,25,500),
  (4,25,500),
  (5,26,500),
  (6,26,1000),
  (7,26,2500),
  (8,26,3340),
  (9,27,350),
  (10,27,800),
  (11,27,1500),
  (12,27,2500),
  (13,27,3100);
COMMIT;

#
# Data for the `niveles_mm` table  (LIMIT 0,500)
#

INSERT INTO `niveles_mm` (`id_niveles`, `vel_entrada`, `vel_salida`, `torque_continuo`, `torque_pico`, `temp_max`, `temp_min`, `flujo_presion_max`, `flujo_presion_min`, `fuerza_max`, `fuerza_min`, `vibracion_max`, `vibracion_min`, `rpms_max`, `fuerzaperilla_max`) VALUES 
  (1,10,35,40,80,75,10,45,1,900,10,80,15,10000,25);
COMMIT;

#
# Data for the `pcalculos` table  (LIMIT 0,500)
#

INSERT INTO `pcalculos` (`id_pCalculos`, `iSampleRate`, `iNumMuestras`, `iCanalesAna`, `iSampleRateWrite`, `dRadio`, `dGravedad`, `dKgcms2ToLbins2`, `dRpmToRadseg2`, `dInerciaMotor`, `dTf`, `fCambioUnidades`) VALUES 
  (1,1000,1000,10,1000,0.5,9.8,0.868,9.55,0,0,1);
COMMIT;

#
# Data for the `pdimension` table  (LIMIT 0,500)
#

INSERT INTO `pdimension` (`id_pDimension`, `fLbtoAmp`, `iEje`, `iDivisionesY`, `iVariables`, `iSegundos`, `iSizeMsj`, `iCantItems`, `iSamplesPlot`) VALUES 
  (1,14.51,0,10,3,5,500,10,100);
COMMIT;

#
# Data for the `pescalas` table  (LIMIT 0,500)
#

INSERT INTO `pescalas` (`id_pescalas`, `dslope_Torque`, `dy0_torque`, `dslope_Nivel`, `dy0_nivel`, `dslop_Temp`, `dy0_temp`, `dslop_Flujo`, `dy0_flujo`, `dslope_Corriente`, `dy0_corriente`) VALUES 
  (1,354.03,5.4,1,0,10,0,3.79,0,2.75,0);
COMMIT;

#
# Data for the `philos_motion` table  (LIMIT 0,500)
#

INSERT INTO `philos_motion` (`id_philos_Motion`, `iTarjetaID`, `iEje`, `iCuentasRev`, `iTimeout`, `iMotionSample`) VALUES 
  (1,1,4,16384,65535,130);
COMMIT;

#
# Data for the `pvalores` table  (LIMIT 0,500)
#

INSERT INTO `pvalores` (`id_pValores`, `dMaxSalAnaVolts`, `dMaxEntAna0`, `dMinEntAna0`, `dMaxEntAna1`, `dMinEntAna1`, `dMaxEntAna2`, `dMinEntAna2`, `dMaxEntAna3`, `dMinEntAna3`, `dMaxEntAna4`, `dMinEntAna4`, `dMaxEntAna5`, `dMinEntAna5`, `dMaxEntAna6`, `dMinEntAna6`, `dMaxEntAna7`, `dMinEntAna7`) VALUES 
  (1,10,1770.15,-1770.15,2,0,100,-10,100,-10,37.9,0,37.9,0,37.9,0,27.5,0);
COMMIT;

#
# Data for the `tipo_secuencia` table  (LIMIT 0,500)
#

INSERT INTO `tipo_secuencia` (`id_tipo_sec`, `fk_id_secuencias`, `no_secuencia`, `pos_inicial`, `pos_final`, `descripcion`, `cicloxmin`, `ciclostotal`, `velocidadm`) VALUES 
  (1,5,1,'R','N','R ---> N ---> R',8,1200,1000),
  (2,5,2,'1','2','1 ---> 2 ---> 1',8,2000,3500),
  (3,6,1,'4','1','4 ---> 1 ---> 4',12,4000,3250),
  (4,6,2,'1','2','1 ---> 2 ---> 1',12,2500,2300),
  (5,6,3,'2','5','2 ---> 5 ---> 2',14,3200,3100),
  (6,6,4,'7','2','7 ---> 2 ---> 7',20,5000,4100),
  (7,7,1,'R','N','R ---> N ---> R',16,20000,1500),
  (8,7,2,'1','6','1 ---> 6 ---> 1',10,2700,4000),
  (9,7,3,'3','7','3 ---> 7 ---> 3',13,4500,3700);
COMMIT;



/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;