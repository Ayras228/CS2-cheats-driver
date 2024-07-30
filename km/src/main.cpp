#include<ntifs.h>

extern "C"
{
	NTKERNELAPI NTSTATUS IoCreateDriver(PUNICODE_STRING DriverName,
										PDRIVER_INITIALIZE InitializationFunction);

	NTKERNELAPI NTSTATUS MmCopyVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress,
											 PEPROCESS TagretProcess, PVOID TagretAddress,
											 SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);
}


void debug_print(PCSTR text)
{
#ifndef DEBUG
	UNREFERENCED_PARAMETER(text);
#endif // DEBUG
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, text));
}
namespace driver
{
	namespace codes
	{
		//userd to setup the driver
		constexpr ULONG attach =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

		constexpr ULONG read =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

		constexpr ULONG write =
			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	}
	//Shared between user mode & kernel mode.
	struct Request 
	{
		HANDLE process_id;
		
		PVOID target;
		PVOID buffer;
		
		SIZE_T size;
		SIZE_T return_size;
	};

	NTSTATUS create(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);

		IoCompleteRequest(irp, IO_NO_INCREMENT);

		return irp->IoStatus.Status;
	}


	NTSTATUS close(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);

		IoCompleteRequest(irp, IO_NO_INCREMENT);

		return irp->IoStatus.Status;
	}

	//Note: Todo
	NTSTATUS device_control(PDEVICE_OBJECT device_object, PIRP irp)
	{
		UNREFERENCED_PARAMETER(device_object);


		debug_print("[+] Device control called.\n");
		 
		NTSTATUS status = STATUS_UNSUCCESSFUL;
		
		//we need this to determine which code was passed through.
		PIO_STACK_LOCATION stack_irp = IoGetCurrentIrpStackLocation(irp);


		//Access the request object sent from user mode.
		auto request = reinterpret_cast<Request*>(irp->AssociatedIrp.SystemBuffer);
		
		if (stack_irp == nullptr || request == nullptr)
		{
			IoCompleteRequest(irp, IO_NO_INCREMENT);
			return status;
		}

		//Target process we want access to.
		static PEPROCESS target_proccess = nullptr;

		const ULONG control_code = stack_irp->Parameters.DeviceIoControl.IoControlCode;
		switch (control_code)
		{
		case codes::attach:
			status = PsLookupProcessByProcessId(request->process_id, &target_proccess);
			break;

		case codes::read:
			if (target_proccess != nullptr)
			{
				status = MmCopyVirtualMemory(target_proccess, request->target,
											 PsGetCurrentProcess(), request->buffer,
											 request->size, KernelMode, &request->return_size);
			}
			break;

		case codes::write:
			if (target_proccess != nullptr)
			{
				status = MmCopyVirtualMemory(PsGetCurrentProcess(), request->buffer,
					target_proccess, request->target,
					request->size, KernelMode, &request->return_size);
			}
			break;

		default:
			break;
		}


		irp->IoStatus.Status = status;
		irp->IoStatus.Information = sizeof(Request);

		IoCompleteRequest(irp, IO_NO_INCREMENT);

		return irp->IoStatus.Status;
	}
}




NTSTATUS driver_main(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(registry_path);

	UNICODE_STRING device_name = {};
	RtlInitUnicodeString(&device_name, L"\\Driver\\SexyDriver");

	//create driver device obj.
	PDEVICE_OBJECT device_object = nullptr;
	NTSTATUS status = IoCreateDevice(driver_object, 0, &device_name, FILE_DEVICE_UNKNOWN,
										FILE_DEVICE_SECURE_OPEN, FALSE, &device_object);
	if (status != STATUS_SUCCESS)
	{
		debug_print("[-] Failed to create driver device.\n");
	}
	debug_print("[+] Driver device successfully created .\n");

	UNICODE_STRING symbolic_link = {};
	RtlInitUnicodeString(&symbolic_link, L"\\DosDevices\\SexyDriver");
	
	status = IoCreateSymbolicLink(&symbolic_link, &device_name);
	if (status != STATUS_SUCCESS)
	{ 
		debug_print("[-] Failed to establish symbolic link.\n");
	}
	debug_print("[+] Driver symbolic link successfully establish .\n");


	//Allow us yo send small amounts of data between um/km.
	SetFlag(device_object->Flags, DO_BUFFERED_IO);

	driver_object->MajorFunction[IRP_MJ_CREATE] = driver::create;
	driver_object->MajorFunction[IRP_MJ_CLOSE] = driver::close;
	driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = driver::device_control;

	ClearFlag(device_object->Flags, DO_DEVICE_INITIALIZING);
	debug_print("[-] Driver initialized successfully.\n");
	return STATUS_SUCCESS;
}


NTSTATUS DriverEntry()
{
	debug_print("[+] Hello from the kernel!\n");
	UNICODE_STRING driver_name = {};
	RtlInitUnicodeString(&driver_name, L"\\Driver\\SexyDriver");

	return IoCreateDriver(&driver_name,&driver_main);
}