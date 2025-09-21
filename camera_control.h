/*
 * camera_control.h
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 */
#ifndef CAMERA_CONTROL_H_
#define CAMERA_CONTROL_H_

#include "sapi.h"
#include "config.h"

#ifdef ENABLE_CAMERA_CONTROL

void camera_control_init(void);
void camera_control_start_stream(void);
void camera_control_stop_stream(void);

#endif /* ENABLE_CAMERA_CONTROL */

#endif /* CAMERA_CONTROL_H_ */
