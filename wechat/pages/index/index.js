//获取应用实例
const app = getApp()

Page({
  data: {
    uid: 'dba6ac8efa663ecaa1a7d6c3d470e5bb',
    topic: "xiaoche",
    device_status: "离线", //默认离线
    powerstatus:"已关闭"   //默认关闭
  },
  forwardclick: function() {
    var that = this
    that.setData({
      powerstatus:"前进"
    })
        //控制接口
        wx.request({
          url: 'https://api.bemfa.com/api/device/v1/data/1/', //api接口，详见接入文档
          method:"POST",
          data: {  //请求字段，详见巴法云接入文档，http接口
            uid: that.data.uid,
            topic: that.data.topic,
            msg:"forward"   //发送消息为forward的消息
          },
          header: {
            'content-type': "application/x-www-form-urlencoded"
          },
          success (res) {
            console.log(res.data)
            wx.showToast({
              title:'前进成功',
              icon:'success',
              duration:1000
            })
          }
        })
  },
   backclick: function() {
    var that = this
    that.setData({
      powerstatus:"后退"
    })
     //控制接口
     wx.request({
      url: 'https://api.bemfa.com/api/device/v1/data/1/', //api接口，详见接入文档
      method:"POST",
      data: {
        uid: that.data.uid,
        topic: that.data.topic,
        msg:"back"
      },
      header: {
        'content-type': "application/x-www-form-urlencoded"
      },
      success (res) {
        console.log(res.data)
        wx.showToast({
          title:'后退成功',
          icon:'success',
          duration:1000
        })
      }
    })
    },
    leftclick: function() {
    var that = this
    that.setData({
      powerstatus:"左行"
    })

        //控制接口
        wx.request({
          url: 'https://api.bemfa.com/api/device/v1/data/1/', //api接口，详见接入文档
          method:"POST",
          data: {  //请求字段，详见巴法云接入文档，http接口
            uid: that.data.uid,
            topic: that.data.topic,
            msg:"left"   //发送消息为left的消息
          },
          header: {
            'content-type': "application/x-www-form-urlencoded"
          },
          success (res) {
            console.log(res.data)
            wx.showToast({
              title:'左行成功',
              icon:'success',
              duration:1000
            })
          }
        })
  },
  rightclick: function() {
    var that = this
    that.setData({
      powerstatus:"右行"
    })

        //控制接口
        wx.request({
          url: 'https://api.bemfa.com/api/device/v1/data/1/', //api接口，详见接入文档
          method:"POST",
          data: {  //请求字段，详见巴法云接入文档，http接口
            uid: that.data.uid,
            topic: that.data.topic,
            msg:"right"   //发送消息为right的消息
          },
          header: {
            'content-type': "application/x-www-form-urlencoded"
          },
          success (res) {
            console.log(res.data)
            wx.showToast({
              title:'右行成功',
              icon:'success',
              duration:1000
            })
          }
        })
  },
  stopclick: function() {
    var that = this
    that.setData({
      powerstatus:"刹车"
    })

        //控制接口
        wx.request({
          url: 'https://api.bemfa.com/api/device/v1/data/1/', //api接口，详见接入文档
          method:"POST",
          data: {  //请求字段，详见巴法云接入文档，http接口
            uid: that.data.uid,
            topic: that.data.topic,
            msg:"stop"   //发送消息为left的消息
          },
          header: {
            'content-type': "application/x-www-form-urlencoded"
          },
          success (res) {
            console.log(res.data)
            wx.showToast({
              title:'刹车成功',
              icon:'success',
              duration:1000
            })
          }
        })
  },
  left_loopclick: function() {
    var that = this
    that.setData({
      powerstatus:"左转"
    })

        //控制接口
        wx.request({
          url: 'https://api.bemfa.com/api/device/v1/data/1/', //api接口，详见接入文档
          method:"POST",
          data: {  //请求字段，详见巴法云接入文档，http接口
            uid: that.data.uid,
            topic: that.data.topic,
            msg:"left_loop"   //发送消息为left_loop的消息
          },
          header: {
            'content-type': "application/x-www-form-urlencoded"
          },
          success (res) {
            console.log(res.data)
            wx.showToast({
              title:'左转成功',
              icon:'success',
              duration:1000
            })
          }
        })
  },
  right_loopclick: function() {
    var that = this
    that.setData({
      powerstatus:"右转"
    })

        //控制接口
        wx.request({
          url: 'https://api.bemfa.com/api/device/v1/data/1/', //api接口，详见接入文档
          method:"POST",
          data: {  //请求字段，详见巴法云接入文档，http接口
            uid: that.data.uid,
            topic: that.data.topic,
            msg:"right_loop"   //发送消息为right_loop的消息
          },
          header: {
            'content-type': "application/x-www-form-urlencoded"
          },
          success (res) {
            console.log(res.data)
            wx.showToast({
              title:'右转成功',
              icon:'success',
              duration:1000
            })
          }
        })
  },
  onLoad: function () {
    var that = this

    //请求设备状态
    //设备断开不会立即显示离线，由于网络情况的复杂性，离线1分钟左右才判断真离线
    wx.request({
      url: 'https://api.bemfa.com/api/device/v1/status/', //状态api接口，详见巴法云接入文档
      data: {
        uid: that.data.uid,
        topic: that.data.topic,
      },
      header: {
        'content-type': "application/x-www-form-urlencoded"
      },
      success (res) {
        console.log(res.data)
        if(res.data.status === "online"){
          that.setData({
            device_status:"在线"
          })
        }else{
          that.setData({
            device_status:"离线"
          })
        }
        console.log(that.data.device_status)
      }
    })

          wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/', //get接口，详见巴法云接入文档
            data: {
              uid: that.data.uid,
              topic: that.data.topic,
            },
            header: {
              'content-type': "application/x-www-form-urlencoded"
            },
            success (res) {
              console.log(res.data)
                that.setData({
                  powerstatus:"前进"
                })
              console.log(that.data.powerstatus)
            }
          })


    //设置定时器，每五秒请求一下设备状态
    setInterval(function () {
      console.log("定时请求设备状态,默认五秒");
      wx.request({
        url: 'https://api.bemfa.com/api/device/v1/status/',  //get 设备状态接口，详见巴法云接入文档
        data: {
          uid: that.data.uid,
          topic: that.data.topic,
        },
        header: {
          'content-type': "application/x-www-form-urlencoded"
        },
        success (res) {
          console.log(res.data)
          if(res.data.status === "online"){
            that.setData({
              device_status:"在线"
            })
          }else{
            that.setData({
              device_status:"离线"
            })
          }
          console.log(that.data.device_status)
        }
      })

      //请求询问设备开关/状态
      wx.request({
        url: 'https://api.bemfa.com/api/device/v1/data/1/', //get接口，详见巴法云接入文档
        data: {
          uid: that.data.uid,
          topic: that.data.topic,
        },
        header: {
          'content-type': "application/x-www-form-urlencoded"
        },
        success (res) {
          console.log(res.data)
          if(res.data.msg === "on"){
            that.setData({
              powerstatus:"已打开"
            })
          }
          console.log(that.data.powerstatus)
        }
      })

    }, 5000)
  }
})
