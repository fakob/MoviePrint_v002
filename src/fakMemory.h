#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include<mach/mach.h>


class fakMemory {
    
public:
    
    fakMemory(){}
    
    vm_size_t page_size;
    vm_statistics_data_t vm_stats;
    
    mach_port_t mach_port = mach_host_self();
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);
    
    
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    
    int getRAMCurrentlyUsedByProcess(){
        if (KERN_SUCCESS != task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count))
        {
            return -1;
        }
        return t_info.resident_size/1024/1024;
        // resident size is in t_info.resident_size;
        // virtual size is in t_info.virtual_size;
    }
    
    int64_t getRAMCurrentlyUsed(){
        int64_t RAMCurrentlyUsed;
        if (KERN_SUCCESS == host_page_size(mach_port, &page_size) && KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO, (host_info_t)&vm_stats, &count))
        {
            RAMCurrentlyUsed = ((int64_t)vm_stats.active_count +
                                (int64_t)vm_stats.inactive_count +
                                (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
        }
        return RAMCurrentlyUsed/1024/1024;
    }
    
    int64_t getRAMCurrentlyFree(){
        int64_t myFreeMemory;
        if (KERN_SUCCESS == host_page_size(mach_port, &page_size) && KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO, (host_info_t)&vm_stats, &count))
        {
            myFreeMemory = (int64_t)vm_stats.free_count * (int64_t)page_size;
            
        }
        return myFreeMemory;
    }
};