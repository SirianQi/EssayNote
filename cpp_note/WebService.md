# WebService

[TOC]

## WebService是什么

广义的webservice可以理解为web应用程序，这里讨论的是基于soap协议的webservice（https://www.w3.org/2002/ws/Activity）。

WebService为各个平台上运行的不同程序之间提供了一个标准的交互方法，是一套RPC框架。WebService与其他rpc服务不同之处在于：WebService在使用的xml格式数据基础之上，还需要严格地根据soap协议描述post报文信息；在提供了基本的服务调用功能之外，也提供了访问“接口文档”的功能。

## 相关定义

数据报文协议：XML，并遵守SOAP协议与WSDL语法。

应用层协议：一般使用HTTP协议中的POST方法。

### SOAP协议

Simple Object Access Protocol（简单对象访问协议）是一个基于xml协议的轻量级结构化数据交换协议，它具有很强的可扩展性，通常用于分散的分布式环境中。简单的理解就是SOAP在xml个格式的基础上定义了属于自己的数据协议，接下来简述一下SOAP基于xml做了哪些事情。

 #### SOAP消息格式

SOAP消息对象包括：Envelope、Header、Body

### WSDL语法



## 使用curl工具模拟调用WebService接口

