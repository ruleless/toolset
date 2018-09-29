#ifndef __COMMAND_FACTORY_H__
#define __COMMAND_FACTORY_H__

#include <iostream>
#include <string>
#include <vector>

class CommandFactory;
class Command
{
  public:
    Command() : factory_(NULL) { }

    virtual ~Command() { }

    // 获取命令名称
    virtual std::string getName() const = 0;

    // 命令的简单描述(单行)
    virtual const char *desc() const = 0;

    // 执行命令接口
    virtual void run(int argc, char *argv[]) = 0;

    // 是否应提示给用户
    virtual bool shouldHint(const std::string &cmdname) const;

    inline const CommandFactory *getFactory() const
    {
        return factory_;
    }
    inline void _setFac(CommandFactory *f)
    {
        factory_ = f;
    }

  protected:
    CommandFactory *factory_;
};

// 工厂基类
class CommandFactory
{
  public:
    virtual std::string getName() = 0;
    virtual Command *getCommand() = 0;
};

// 具体工厂
template <class CommandType>
class CommandFactoryImpl : public CommandFactory
{
  public:
    virtual std::string getName()
    {
        return getCommand()->getName();
    }

    virtual Command *getCommand()
    {
        static CommandType s_cmd;
        if (!s_cmd.getFactory()) {
            s_cmd._setFac(this);
        }
        return &s_cmd;
    }
};

// 工厂注册类
typedef std::vector<CommandFactory *> FactoryList;
class CommandFactoryRegistry
{
  public:
    CommandFactoryRegistry();
    ~CommandFactoryRegistry();

    static CommandFactoryRegistry *getRegistry();
    static bool isValid();

    void registerFactory(CommandFactory *factory);
    void unregisterFactory(CommandFactory *factory);

    CommandFactory *getCommandFactory(const std::string &cmdName);

    inline FactoryList::iterator begin()
    {
        return factories_.begin();
    }
    inline FactoryList::iterator end()
    {
        return factories_.end();
    }

  protected:
    FactoryList factories_;
    static bool valid_;
};

// AutoRegisterCommand用于实现编译期注册，
// 编译期注册的好处是，新加入的命令不需要在现有代码里做任何改动
template<class CommandType>
class AutoRegisterCommand
{
  public:
    AutoRegisterCommand()
            : registry_(CommandFactoryRegistry::getRegistry())
    {
        registry_->registerFactory(&factory_);
    }

    ~AutoRegisterCommand()
    {
        if (CommandFactoryRegistry::isValid()) {
            registry_->unregisterFactory(&factory_);
        }
    }

  private:
    CommandFactoryRegistry *registry_;
    CommandFactoryImpl<CommandType> factory_;
};


#define COMMAND_FACTORY_REGISTRATION(CommandType)                       \
    static AutoRegisterCommand<CommandType> _autoRegisterRegistry_##CommandType

#define RUN_F(CommandType, CommandName, descstr)                        \
    class _Impl##CommandType##CommandName : public CommandType {        \
      public:                                                           \
      virtual std::string getName() const { return #CommandName; }      \
      virtual const char *desc() const { return descstr; }              \
      virtual void run(int argc, char *argv[]);                         \
    };                                                                  \
    COMMAND_FACTORY_REGISTRATION(_Impl##CommandType##CommandName);      \
    void _Impl##CommandType##CommandName::run(int argc, char *argv[])

#define RUN(CommandName, descstr) RUN_F(Command, CommandName, descstr)

#endif // __COMMAND_FACTORY_H__
