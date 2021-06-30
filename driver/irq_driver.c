#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define IRQ_PIN 91
#define IRQ_NAME "test_irq"
#define IRQ_COOKIE "test_irq_module"

static unsigned long flags = 0;

MODULE_LICENSE("GLP");
MODULE_AUTHOR("Bojan Faletic");
MODULE_DESCRIPTION("Test PL-PS IRQ");

static irqreturn_t button_isr(int irq, void *data) {
  local_irq_save(flags);
  printk("interrupt\n");
  local_irq_restore(flags);
  return IRQ_HANDLED;
}

int init_module(void) {
  printk("module init\n");

  if (request_irq(IRQ_PIN, button_isr, IRQF_TRIGGER_RISING, IRQ_NAME,
                  IRQ_COOKIE)) {
    return -1;
  }
  return 0;
}

void cleanup_module(void){
    free_irq(IRQ_PIN, IRQ_COOKIE);
}