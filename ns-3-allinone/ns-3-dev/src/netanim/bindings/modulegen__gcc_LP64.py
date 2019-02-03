from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers


import pybindgen.settings
import warnings

class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, wrapper, exception, traceback_):
        warnings.warn("exception %r in wrapper %s" % (exception, wrapper))
        return True
pybindgen.settings.error_handler = ErrorHandler()


import sys

def module_init():
    root_module = Module('ns.netanim', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    root_module = module.get_root()
    
    ## ff-mac-common.h (module 'lte'): ns3::Result_e [enumeration]
    module.add_enum('Result_e', ['SUCCESS', 'FAILURE'], import_from_module='ns.lte')
    ## ff-mac-common.h (module 'lte'): ns3::SetupRelease_e [enumeration]
    module.add_enum('SetupRelease_e', ['setup', 'release'], import_from_module='ns.lte')
    ## ff-mac-common.h (module 'lte'): ns3::CeBitmap_e [enumeration]
    module.add_enum('CeBitmap_e', ['TA', 'DRX', 'CR'], import_from_module='ns.lte')
    ## ff-mac-common.h (module 'lte'): ns3::NormalExtended_e [enumeration]
    module.add_enum('NormalExtended_e', ['normal', 'extended'], import_from_module='ns.lte')
    ## log.h (module 'core'): ns3::LogLevel [enumeration]
    module.add_enum('LogLevel', ['LOG_NONE', 'LOG_ERROR', 'LOG_LEVEL_ERROR', 'LOG_WARN', 'LOG_LEVEL_WARN', 'LOG_DEBUG', 'LOG_LEVEL_DEBUG', 'LOG_INFO', 'LOG_LEVEL_INFO', 'LOG_FUNCTION', 'LOG_LEVEL_FUNCTION', 'LOG_LOGIC', 'LOG_LEVEL_LOGIC', 'LOG_ALL', 'LOG_LEVEL_ALL', 'LOG_PREFIX_FUNC', 'LOG_PREFIX_TIME', 'LOG_PREFIX_NODE', 'LOG_PREFIX_LEVEL', 'LOG_PREFIX_ALL'], import_from_module='ns.core')
    ## address.h (module 'network'): ns3::Address [class]
    module.add_class('Address', import_from_module='ns.network')
    ## address.h (module 'network'): ns3::Address::MaxSize_e [enumeration]
    module.add_enum('MaxSize_e', ['MAX_SIZE'], outer_class=root_module['ns3::Address'], import_from_module='ns.network')
    ## eps-bearer.h (module 'lte'): ns3::AllocationRetentionPriority [struct]
    module.add_class('AllocationRetentionPriority', import_from_module='ns.lte')
    ## animation-interface.h (module 'netanim'): ns3::AnimationInterface [class]
    module.add_class('AnimationInterface')
    ## animation-interface.h (module 'netanim'): ns3::AnimationInterface::CounterType [enumeration]
    module.add_enum('CounterType', ['UINT32_COUNTER', 'DOUBLE_COUNTER'], outer_class=root_module['ns3::AnimationInterface'])
    typehandlers.add_type_alias(u'ns3::AnimationInterface::CounterType', u'ns3::AnimationInterface::CounterType')
    typehandlers.add_type_alias(u'ns3::AnimationInterface::CounterType*', u'ns3::AnimationInterface::CounterType*')
    typehandlers.add_type_alias(u'ns3::AnimationInterface::CounterType&', u'ns3::AnimationInterface::CounterType&')
    typehandlers.add_type_alias(u'void ( * ) ( char const * )', u'ns3::AnimationInterface::AnimWriteCallback')
    typehandlers.add_type_alias(u'void ( * ) ( char const * )*', u'ns3::AnimationInterface::AnimWriteCallback*')
    typehandlers.add_type_alias(u'void ( * ) ( char const * )&', u'ns3::AnimationInterface::AnimWriteCallback&')
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList [class]
    module.add_class('AttributeConstructionList', import_from_module='ns.core')
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item [struct]
    module.add_class('Item', import_from_module='ns.core', outer_class=root_module['ns3::AttributeConstructionList'])
    typehandlers.add_type_alias(u'std::list< ns3::AttributeConstructionList::Item > const_iterator', u'ns3::AttributeConstructionList::CIterator')
    typehandlers.add_type_alias(u'std::list< ns3::AttributeConstructionList::Item > const_iterator*', u'ns3::AttributeConstructionList::CIterator*')
    typehandlers.add_type_alias(u'std::list< ns3::AttributeConstructionList::Item > const_iterator&', u'ns3::AttributeConstructionList::CIterator&')
    ## spectrum-model.h (module 'spectrum'): ns3::BandInfo [struct]
    module.add_class('B