/**
 * @file task.h
 * @author dokee (dokee.39@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _TIMESLICE_TASK_H
#define _TIMESLICE_TASK_H

#include "list.h"

typedef enum {
    TASK_STOP,
    TASK_RUN
} IsTaskRun;

typedef struct timesilce_task
{
    unsigned int id;
    void (*task_hdl)(void);
    IsTaskRun is_run;
    unsigned int timer;
    unsigned int timeslice_len;
    ListObj timeslice_task_list;
} TimesilceTaskObj;

void timeslice_exec(void);
void timeslice_tick(void);
void timeslice_task_init(TimesilceTaskObj* obj, void (*task_hdl)(void), unsigned int id, unsigned int timeslice_len);
void timeslice_task_add(TimesilceTaskObj* obj);
void timeslice_task_del(TimesilceTaskObj* obj);
unsigned int timeslice_get_task_timeslice_len(TimesilceTaskObj* obj);
unsigned int timeslice_get_task_num(void);
unsigned char timeslice_task_isexist(TimesilceTaskObj* obj);

#endif