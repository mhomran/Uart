/**
 * @file det.h
 * @author Mohamed Hassanin
 * @brief a default error tracer for error handling
 * @version 0.1
 * @date 2021-03-12
 */
#ifndef DET_H
#define DET_H
/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
/******************************************************************************
 * Function prototypes
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

extern void Det_ReportError(uint16_t ModuleId, uint8_t InstanceId, uint8_t ApiId, uint8_t ErrorId);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* DET_H */
/*****************************End of File ************************************/
